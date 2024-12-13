// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseRangedWeapon.generated.h"

class UArrowComponent;
UCLASS()
class ECHOOFTHEPAST_API ABaseRangedWeapon : public ABaseWeapon
{
	GENERATED_BODY()

	ABaseRangedWeapon();
	
	UPROPERTY(EditAnywhere, Category="Default")
	double FireRate;

	UPROPERTY(EditAnywhere, Category="Default")
	TObjectPtr<UClass> ProjectileClass;

	UFUNCTION()
	void SpawnProjectile();

	FTimerHandle TimerHandle;
	bool bHasExecuted = false;
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UArrowComponent> FireSocket;

	virtual void Attack(const bool IsStart) override;
};
