// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ECHOOFTHEPAST_API ABaseMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Default")
	double DamageAmount;
public:
	virtual void Attack(const bool IsStart) override;

};
