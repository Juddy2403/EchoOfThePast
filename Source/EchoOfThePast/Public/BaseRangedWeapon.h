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

	UFUNCTION()
	void SpawnParticlesAtMuzzle(UParticleSystem* ParticleSystem) const;
	
	FTimerHandle TimerHandle;
	FTimerHandle TimerExecutedHandle;
	bool bHasStartedShootingLoop = false;
	bool bCanShoot = true;

protected:
	ABaseRangedWeapon();
	int CurrentAmmo = 0;

	UFUNCTION()
	virtual bool GetProjectileTargetLocation(FVector& targetLocation) const;

	UFUNCTION()
	void SpawnProjectile();
	
	UPROPERTY(EditAnywhere, Category="Default")
	TSubclassOf<ABaseProjectile> ProjectileClass;
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	double FireRate;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UArrowComponent> FireSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UParticleSystem* SparksParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UParticleSystem* SmokeParticles;

	virtual void Attack(const bool IsStart, const float DamageModifier = 1) override;

	UFUNCTION(BlueprintCallable, Category="Default")
	void SetAmmo(const int AmmoNumber) { CurrentAmmo = AmmoNumber; }
	
	UFUNCTION(BlueprintCallable, Category="Default")
	int GetAmmo() const { return CurrentAmmo; }

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBulletFired);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnBulletFired OnBulletFired;
};
