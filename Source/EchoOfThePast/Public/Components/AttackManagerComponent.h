// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackManagerComponent.generated.h"


enum class EDamageTypeEnum : uint8;
class ABaseWeapon;
class ABaseMeleeWeapon;
class ABaseRangedWeapon;
class UNiagaraSystem;
class USoundCue;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOOFTHEPAST_API UAttackManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttackManagerComponent();

	UPROPERTY(BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<ABaseRangedWeapon> EquippedGun;

	UPROPERTY(BlueprintReadWrite, Category = "Weapons")
	TSubclassOf<ABaseMeleeWeapon> EquippedSword;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
	TObjectPtr<ABaseWeapon> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	int PowerfulComboHigherThan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> PunchMontages;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	TObjectPtr<USoundCue> AttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> MeleeVfx;
	
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float ComboModifier;
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float DamageModifier = 1.0f;

	UFUNCTION(BlueprintCallable, Category="Weapons")
	void EquipWeapon(const TSubclassOf<ABaseWeapon>& WeaponClass);

	UFUNCTION(BlueprintCallable, Category="Weapons")
	void SwitchAttackType(const EDamageTypeEnum Type);
	
	UFUNCTION(BlueprintCallable, Category="Attack")
	void ResetAttack() { bCanAttack = true; }

	UFUNCTION(BlueprintCallable, Category="Attack")
	void Attack(bool bIsStart);

	UFUNCTION(BlueprintCallable, Category="Attack")
	void PerformPunchAttack(const FName& SocketName = NAME_None);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;

	int CurrentCombo = 0;
	int CurrentPunchCombo = 0;
	bool bCanAttack = true;
	float LastAttackTime = 0.0f;
	float SpeedModifier = 1.0f;

	void DropOldWeapon(const EDamageTypeEnum& Type);
	void SwitchWeapon(const TSubclassOf<ABaseWeapon>& WeaponClass, bool bIsSwitchingToMelee = false);
	UAnimMontage* GetMeleeMontage(const float& WeaponPlayRate);
	UAnimMontage* GetPunchingMontage();
	void PerformMeleeAttack();
	void PerformRangedAttack(const bool& bIsStart);
};
