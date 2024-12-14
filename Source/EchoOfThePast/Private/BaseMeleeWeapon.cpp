// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMeleeWeapon.h"

#include "DamageableInterface.h"
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

	FVector StartSocketLocation;
	FVector EndSocketLocation;
	if (WeaponMesh->DoesSocketExist("Start")) StartSocketLocation = WeaponMesh->GetSocketLocation("Start");
	else UE_LOG(LogTemp, Warning, TEXT("Socket does not exist: Start"));
	if (WeaponMesh->DoesSocketExist("End")) EndSocketLocation = WeaponMesh->GetSocketLocation("End");
	else UE_LOG(LogTemp, Warning, TEXT("Socket does not exist: End"));

	// Parameters
	float Radius = 10.0f; // Sphere radius

	// Object types to trace
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Trace parameters
	TArray<AActor*> IgnoredActors;
	TArray<FHitResult> HitResults;
	constexpr bool bTraceComplex = false; // Whether to trace against complex collision

	// Perform the trace
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
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
	// Process results
	if (!bHit) return;
	for (const FHitResult& Hit : HitResults)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			//verify if hit actor has the same tag as the attach parent actor
			if (GetAttachParentActor()->Tags.Num() != 0 && HitActor->ActorHasTag(GetAttachParentActor()->Tags[0]))
				continue;

			//get UHealth component from the hit actor
			UHealth* HealthComponent = HitActor->FindComponentByClass<UHealth>();
			if (HealthComponent)
			{
				bool isDead = false;
				HealthComponent->DoDamage_Implementation(DamageAmount, isDead);
			}
		}
	}
}
