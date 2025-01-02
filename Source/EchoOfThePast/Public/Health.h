// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageableInterface.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) 
class ECHOOFTHEPAST_API UHealth : public UActorComponent, public IDamageableInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void Heal(double Amount);
	
	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetMaxHP(double MaxHP);

	/** Please add a function description */
	virtual void DoDamage_Implementation(float amount, bool isCrit, bool& isDead) override;
	
	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnDeath OnDeath;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamage, float, DamageAmount, bool, IsCrit);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnDamage OnDamage;

	UPROPERTY(EditDefaultsOnly ,BlueprintReadWrite ,Category="Health")
	double MaxHealth;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite ,Category="Health")
	double CurrentHealth;
	
	UPROPERTY(EditDefaultsOnly ,BlueprintReadWrite ,Category="Default")
	double InvincibilityTime;
private:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category="Default")
	TObjectPtr<USoundBase> HitSound;

	/** Please add a variable description */
	UPROPERTY()
	bool CanBeHit;
	
	/** For the invincibility period */
	UFUNCTION()
	void ResetCanBeHit();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
};
