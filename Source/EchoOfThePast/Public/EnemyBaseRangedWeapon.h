// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseRangedWeapon.h"
#include "EnemyBaseRangedWeapon.generated.h"

/**
 * 
 */

UCLASS()
class ECHOOFTHEPAST_API AEnemyBaseRangedWeapon : public ABaseRangedWeapon
{
	GENERATED_BODY()
protected:
	virtual bool GetProjectileTargetLocation(FVector& targetLocation) const override;

public:
	virtual void Attack(const bool IsStart, const float DamageModifier = 1) override;

};
