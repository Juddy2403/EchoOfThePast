// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseRangedWeapon.h"
#include "Kismet/GameplayStatics.h"

bool AEnemyBaseRangedWeapon::GetProjectileTargetLocation(FVector& targetLocation) const
{
	targetLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
	return true;
}

void AEnemyBaseRangedWeapon::Attack(const bool IsStart, const float DamageModifier)
{
	SpawnProjectile();
}
