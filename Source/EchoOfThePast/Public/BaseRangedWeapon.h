// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BaseRangedWeapon.generated.h"

class ABaseProjectile;
class UArrowComponent;
UCLASS()
class ECHOOFTHEPAST_API ABaseRangedWeapon : public ABaseWeapon
{
	GENERATED_BODY()

	UFUNCTION()
	void ResetCanShoot() { bCanShoot = true; }
	
	FTimerHandle TimerHandle;
	FTimerHandle TimerExecutedHandle;
	bool bHasStartedShootingLoop = false;
	bool bCanShoot = true;

protected:
	ABaseRangedWeapon();

	UFUNCTION()
	virtual bool GetProjectileTargetLocation(FVector& targetLocation) const;

	UFUNCTION()
	void SpawnProjectile() const;
	
	UPROPERTY(EditAnywhere, Category="Default")
	TSubclassOf<ABaseProjectile> ProjectileClass;
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	double FireRate;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UArrowComponent> FireSocket;

	virtual void Attack(const bool IsStart) override;
};
