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

	bool ExtractHitResult(TArray<FHitResult>& HitResults) const;
	float ComputeDamageAmount() const;
public:
	ABaseMeleeWeapon();
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	float Speed;
	
	virtual void Attack(const bool IsStart, const float DamageModifier = 1) override;

	UFUNCTION(BlueprintCallable)
	float GetPlayRate() const;
};
