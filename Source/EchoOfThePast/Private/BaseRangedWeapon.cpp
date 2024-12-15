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

void ABaseRangedWeapon::SpawnProjectile()
{
	UGameplayStatics::SpawnSoundAttached(AttackSound, WeaponMesh);

	// Get the player controller
	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController) UE_LOG(LogTemp, Warning, TEXT("PlayerController is null."));

	// Perform a hit test under the cursor
	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult))
	{
		// Get the location of the hit result
		const FVector HitLocation = HitResult.Location;

		const FVector ComponentLocation = FireSocket->GetComponentLocation();

		// Calculate the look-at rotation
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ComponentLocation, HitLocation);

		// Create a spawn transform
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(ComponentLocation); // Use the component's location
		SpawnTransform.SetRotation(FQuat(FRotator(0, LookAtRotation.Yaw, 0))); // Only the yaw component

		// Spawn the actor
		if (ProjectileClass)
		{
			ABaseProjectile* Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>( ProjectileClass, SpawnTransform);
			if (Projectile)
			{
				Projectile->DamageAmount = DamageAmount;
				Projectile->CritRate = CritRate;
				if (GetAttachParentActor()->Tags.Num() != 0) Projectile->IgnoreTag = GetAttachParentActor()->Tags[0];
				UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No hit result under cursor."));
	}
}

void ABaseRangedWeapon::Attack(const bool IsStart)
{
	Super::Attack(IsStart);

	if (IsStart)
	{
		if (!bHasExecuted)
		{
			bHasExecuted = true;
			SpawnProjectile();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABaseRangedWeapon::SpawnProjectile,
			                                       1.0 / FireRate, true);
			SpawnProjectile();
		}
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		bHasExecuted = false;
	}
}
