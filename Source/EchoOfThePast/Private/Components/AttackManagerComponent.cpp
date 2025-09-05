// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttackManagerComponent.h"

#include "Components/HealthComponent.h"
#include "Components/LevelManagerComponent.h"
#include "DataTypes/DamageTypeEnum.h"
#include "DataTypes/LevelConfigStruct.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapons/BaseMeleeWeapon.h"
#include "Weapons/BaseRangedWeapon.h"
#include "NiagaraFunctionLibrary.h"
// Sets default values for this component's properties
UAttackManagerComponent::UAttackManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UAttackManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	FSLevelConfigStruct* LevelConfig = UGameplayStatics::GetGameMode(GetWorld())->
	                                   GetComponentByClass<ULevelManagerComponent>()->GetCurrentLevelConfig();
	// The modifiers only apply to the player character
	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == OwnerCharacter)
	{
		DamageModifier = LevelConfig->DamageModifier;
		SpeedModifier = LevelConfig->SpeedModifier;
	}
}

void UAttackManagerComponent::EquipWeapon(const TSubclassOf<ABaseWeapon>& WeaponClass)
{
	const EDamageTypeEnum Type = WeaponClass->GetDefaultObject<ABaseWeapon>()->Type;
	DropOldWeapon(Type);
	if (IsValid(CurrentWeapon) && CurrentWeapon->Type == Type || !IsValid(CurrentWeapon)) SwitchWeapon(WeaponClass);
	switch (Type)
	{
	case EDamageTypeEnum::Melee:
		EquippedSword = WeaponClass;
		break;
	case EDamageTypeEnum::Ranged:
		EquippedGun = WeaponClass;
		break;
	default: break;
	}
}

void UAttackManagerComponent::SwitchAttackType(const EDamageTypeEnum Type)
{
	switch (Type)
	{
	case EDamageTypeEnum::Melee:
		SwitchWeapon(EquippedSword, true);
		break;
	case EDamageTypeEnum::Ranged:
		SwitchWeapon(EquippedGun);
		break;
	case EDamageTypeEnum::None:
		break;
	}
}

void UAttackManagerComponent::DropOldWeapon(const EDamageTypeEnum& Type)
{
	const TSubclassOf<ABaseWeapon> OldWeaponClass = Type == EDamageTypeEnum::Melee ? EquippedSword : EquippedSword;
	if (!IsValid(OldWeaponClass)) return;
	const TSubclassOf<AActor> PickupClass = OldWeaponClass->GetDefaultObject<ABaseWeapon>()->WeaponPickupClass;
	const FVector SpawnLocation = OwnerCharacter->GetActorLocation() + FVector(0, 0, 100);
	GetWorld()->SpawnActor(PickupClass, &SpawnLocation);
}

void UAttackManagerComponent::SwitchWeapon(const TSubclassOf<ABaseWeapon>& WeaponClass, bool bIsSwitchingToMelee)
{
	if (bIsSwitchingToMelee && !IsValid(EquippedSword))
	{
		if (IsValid(CurrentWeapon)) CurrentWeapon->Destroy();
		return;
	}
	if (!IsValid(WeaponClass)) return;
	if (IsValid(CurrentWeapon)) CurrentWeapon->Destroy();
	FActorSpawnParameters Params;
	Params.Owner = OwnerCharacter;
	CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(
		WeaponClass,
		FTransform::Identity,
		Params);
	CurrentWeapon->AttachToComponent(OwnerCharacter->GetMesh(),
	                                 FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                                 CurrentWeapon->SocketName);
}

UAnimMontage* UAttackManagerComponent::GetMeleeMontage(const float& WeaponPlayRate)
{
	const float CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	if (CurrentCombo >= AttackMontages.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentCombo exceeded the number of attack montages. Resetting combo."));
		CurrentCombo = -1;
	}
	const float elapsedTime = CurrentTime - LastAttackTime;
	const float attackInterval = (1.f / WeaponPlayRate) * 2.f; 
	if (elapsedTime > attackInterval)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attack too soon. Resetting combo."));
		CurrentCombo = -1;
	}
	++CurrentCombo;
	LastAttackTime = CurrentTime;
	ComboModifier = CurrentCombo > PowerfulComboHigherThan ? 1.5f : 1.f;
	return AttackMontages.IsValidIndex(CurrentCombo) ? AttackMontages[CurrentCombo] : nullptr;
}

UAnimMontage* UAttackManagerComponent::GetPunchingMontage()
{
	if (CurrentPunchCombo > PunchMontages.Num()) CurrentPunchCombo = -1;
	++CurrentPunchCombo;
	return PunchMontages.IsValidIndex(CurrentPunchCombo) ? PunchMontages[CurrentPunchCombo] : nullptr;
}

void UAttackManagerComponent::PerformMeleeAttack()
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
	if (CurrentMontage != nullptr && !bCanAttack) return;
	OwnerCharacter->MakeNoise(0.5f);
	if (IsValid(MeleeVfx))
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(MeleeVfx, OwnerCharacter->GetMesh(), NAME_None, FVector(0.f), FRotator(0.f), EAttachLocation::Type::KeepRelativeOffset, true);
	}
	ABaseMeleeWeapon* MeleeWeapon = Cast<ABaseMeleeWeapon>(CurrentWeapon);
	if (IsValid(MeleeWeapon))
	{
		float WeaponPlayRate = MeleeWeapon->GetPlayRate() * SpeedModifier;
		UAnimMontage* Montage = GetMeleeMontage(WeaponPlayRate);
		if (Montage)
		{
			AnimInstance->Montage_Play(Montage, WeaponPlayRate);
			bCanAttack = false;
			if (IsValid(AttackSound))
				UGameplayStatics::SpawnSoundAttached(AttackSound, OwnerCharacter->GetMesh());
		}
	}
	else
	{
		UAnimMontage* Montage = GetPunchingMontage();
		if (Montage)
		{
			AnimInstance->Montage_Play(Montage, SpeedModifier);
			bCanAttack = false;
		}
	}
}

void UAttackManagerComponent::PerformRangedAttack(const bool& bIsStart)
{
	if (!IsValid(CurrentWeapon)) return;
	OwnerCharacter->MakeNoise();
	CurrentWeapon->Attack(bIsStart, DamageModifier);
}

void UAttackManagerComponent::Attack(bool bIsStart)
{
	UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(DodgeMontage)) return;
	if (!IsValid(CurrentWeapon) || CurrentWeapon->Type == EDamageTypeEnum::Melee) PerformMeleeAttack();
	else PerformRangedAttack(bIsStart);
}

void UAttackManagerComponent::PerformPunchAttack(const FName& SocketName)
{
	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(SocketName);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<FHitResult> OutHits;

	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		SocketLocation,
		SocketLocation,
		15.f,
		ObjectTypes,
		false, // bTraceComplex
		TArray<AActor*>(), // Actors to ignore
		EDrawDebugTrace::None, // Debug drawing
		OutHits,
		true // Ignore self
	);
	if (!bHit) return;
	for (const FHitResult& Hit : OutHits)
	{
		// Check if actor has the same tag as owner
		AActor* HitActor = Hit.GetActor();
		if (HitActor && OwnerCharacter->Tags.Num() > 0 && !HitActor->ActorHasTag(OwnerCharacter->Tags[0]))
		{
			UHealthComponent* HealthComp = HitActor->GetComponentByClass<UHealthComponent>();
			if (!HealthComp) continue;
			bool IsDead;
			IDamageableInterface::Execute_DoDamage(HealthComp, DamageModifier * 5.f, false, IsDead);
		}
	}
}
