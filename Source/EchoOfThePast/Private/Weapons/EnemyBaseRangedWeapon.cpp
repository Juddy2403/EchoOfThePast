// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/EnemyBaseRangedWeapon.h"

#include "Components/AmmoManagerComponent.h"
#include "Kismet/GameplayStatics.h"

bool AEnemyBaseRangedWeapon::GetProjectileTargetLocation(FVector& targetLocation) const
{
	targetLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	return true;
}

AEnemyBaseRangedWeapon::AEnemyBaseRangedWeapon(): ABaseRangedWeapon()
{
	AmmoManagerComponent = CreateDefaultSubobject<UAmmoManagerComponent>(TEXT("AmmoManagerComponent"));
}

void AEnemyBaseRangedWeapon::Attack(const bool IsStart, const float DamageModifier)
{
	// The enemy doesn't need ammo. This is to make sure he shoots every time
	AmmoManagerComponent->AddAmmo(1);
	SpawnProjectile();
}
