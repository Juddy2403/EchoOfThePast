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

void ABaseRangedWeapon::SpawnProjectile() const
{
	FVector TargetLocation;
	
	if (bShootTowardsPlayer) TargetLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	else
	{
		// Get the player controller
		const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (!PlayerController) UE_LOG(LogTemp, Warning, TEXT("PlayerController is null."));

		// Perform a hit test under the cursor
		FHitResult HitResult;
		if (PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, HitResult))
		{
			// Get the location of the hit result
			TargetLocation = HitResult.Location;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No hit result under cursor."));
			return;
		}
	}
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
		}
	}
	else
	{
		// enemy will always shoot with IsStart = false. This is for the enemy attack only
		if (!bHasExecuted)
		{
			bShootTowardsPlayer = true;
			SpawnProjectile();
			return;
		}
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		bHasExecuted = false;
	}
}
