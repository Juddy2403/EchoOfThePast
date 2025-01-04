// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseRangedWeapon.h"
#include "BaseProjectile.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABaseRangedWeapon::ABaseRangedWeapon()
{
	FireSocket = CreateDefaultSubobject<UArrowComponent>(TEXT("FireSocket"));
	FireSocket->SetupAttachment(RootComponent);
}

bool ABaseRangedWeapon::GetProjectileTargetLocation(FVector& targetLocation) const
{
	// Get the player controller
	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is null."));
		return false;
	}

	// Perform a hit test under the cursor
	FHitResult HitResult;
	ETraceTypeQuery TraceChannel = UCollisionProfile::Get()->ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	if (PlayerController->GetHitResultUnderCursorByChannel(TraceChannel, false, HitResult))
	{
		// Get the location of the hit result
		targetLocation = HitResult.Location;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("No hit result under cursor."));
	return false;
}

void ABaseRangedWeapon::SpawnProjectile() const
{
	FVector TargetLocation;
	if (!GetProjectileTargetLocation(TargetLocation)) return;

	UGameplayStatics::SpawnSoundAttached(AttackSound, WeaponMesh);

	const FVector ComponentLocation = FireSocket->GetComponentLocation();

	// Calculate the look-at rotation
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ComponentLocation, TargetLocation);

	// Create a spawn transform
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ComponentLocation); // Use the component's location
	SpawnTransform.SetRotation(FQuat(FRotator(0, LookAtRotation.Yaw, 0))); // Only the yaw component

	// Spawn the actor
	if (ProjectileClass)
	{
		if (ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileClass, SpawnTransform))
		{
			Projectile->DamageAmount = CurrentDamageModifier * DamageAmount;
			Projectile->CritRate = CritRate;
			if (GetAttachParentActor()->Tags.Num() != 0) Projectile->IgnoreTag = GetAttachParentActor()->Tags[0];
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is null."));
}

void ABaseRangedWeapon::Attack(const bool IsStart, const float DamageModifier)
{
	Super::Attack(IsStart);
	CurrentDamageModifier = DamageModifier;
	if (IsStart)
	{
		if (!bCanShoot) return;
		if (bHasStartedShootingLoop) return;
		bCanShoot = false;
		bHasStartedShootingLoop = true;
		SpawnProjectile();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseRangedWeapon::SpawnProjectile,
		                                       1.0 / FireRate, true);
		GetWorld()->GetTimerManager().SetTimer(TimerExecutedHandle, this, &ABaseRangedWeapon::ResetCanShoot,
		                                       1.0 / FireRate, false);
	}
	else
	{
		bHasStartedShootingLoop = false;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}
