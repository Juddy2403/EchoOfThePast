// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/DamageTypeEnum.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ECHOOFTHEPAST_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	// The skeletal mesh of the weapon
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	FName SocketName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Default")
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY()
	float CurrentDamageModifier = 1;

	
public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	double DamageAmount;
	
	UPROPERTY(BlueprintReadOnly)
	EDamageTypeEnum Type = EDamageTypeEnum::None;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	float CritRate;
	// Attack event
	UFUNCTION(BlueprintCallable, Category="Default")
	virtual void Attack(const bool IsStart, const float DamageModifier = 1){}
};
