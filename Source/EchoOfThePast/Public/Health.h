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
	virtual void DoDamage_Implementation(float amount, bool& isDead) override;
	
	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnDeath OnDeath;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamage);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnDamage OnDamage;

private:
	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, Category="Default")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="Health")
	double MaxHealth;

	UPROPERTY()
	double InvincibilityTime;

	/** Please add a variable description */
	UPROPERTY()
	double CurrentHealth;

	/** Please add a variable description */
	UPROPERTY()
	bool CanBeHit;
	
	/** For the invincibility period */
	UFUNCTION()
	void ResetCanBeHit();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

// public:	
// 	// Called every frame
// 	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
