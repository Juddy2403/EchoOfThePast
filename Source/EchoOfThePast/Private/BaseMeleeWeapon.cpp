// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeWeapon.h"
#include "Health.h"
#include "Kismet/GameplayStatics.h"

void ABaseMeleeWeapon::Attack(const bool IsStart)
{
	Super::Attack(IsStart);
	if (IsStart)
	{
		UGameplayStatics::SpawnSoundAttached(AttackSound, WeaponMesh);
		return;
	}

	// If it hit nothing, return
	TArray<FHitResult> HitResults;
	if (!ExtractHitResult(HitResults)) return;
	
	for (const FHitResult& Hit : HitResults)
	{
		if (const AActor* HitActor = Hit.GetActor())
		{
			//verify if hit actor has the same tag as the attach parent actor
			if (GetAttachParentActor()->Tags.Num() != 0 && HitActor->ActorHasTag(GetAttachParentActor()->Tags[0]))
				continue;

			//get UHealth component from the hit actor
			if (UHealth* HealthComponent = HitActor->FindComponentByClass<UHealth>())
			{
				bool bIsDead = false;
				float damageAmount = ComputeDamageAmount();
				HealthComponent->DoDamage_Implementation(damageAmount, damageAmount > DamageAmount, bIsDead);
			}
		}
	}
}

float ABaseMeleeWeapon::GetPlayRate() const
{
	//mathematical function so that a speed of one means 0.6 play rate and a speed of 100 is 3.0 play rate
	return 0.02424f * Speed + 0.57576f;
}

bool ABaseMeleeWeapon::ExtractHitResult(TArray<FHitResult>& HitResults) const
{
	FVector StartSocketLocation;
	FVector EndSocketLocation;
	if (WeaponMesh->DoesSocketExist("Start")) StartSocketLocation = WeaponMesh->GetSocketLocation("Start");
	else UE_LOG(LogTemp, Error, TEXT("Socket does not exist: Start"));
	if (WeaponMesh->DoesSocketExist("End")) EndSocketLocation = WeaponMesh->GetSocketLocation("End");
	else UE_LOG(LogTemp, Error, TEXT("Socket does not exist: End"));

	// Parameters
	float Radius = 10.0f; // Sphere radius

	// Object types to trace
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Trace parameters
	TArray<AActor*> IgnoredActors;
	constexpr bool bTraceComplex = false; // Whether to trace against complex collision

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		StartSocketLocation,
		EndSocketLocation,
		Radius,
		ObjectTypes,
		bTraceComplex,
		IgnoredActors,
		EDrawDebugTrace::None, // Debug type (None, ForOneFrame, ForDuration)
		HitResults,
		true // Ignore Self
	);
	return bHit;
}

float ABaseMeleeWeapon::ComputeDamageAmount() const
{
	const float CritChance = FMath::FRandRange(0.0f, 100.0f);
	if (CritChance <= CritRate)
	{
		const float CritMultiplier = FMath::FRandRange(1.5f, 2.0f);
		// floor the result to avoid floating point errors
		
		return FMath::RoundToFloat(DamageAmount * CritMultiplier);
	}
	return DamageAmount;
}
