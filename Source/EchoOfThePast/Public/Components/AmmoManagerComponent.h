// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoManagerComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOOFTHEPAST_API UAmmoManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAmmoManagerComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmoChanged, int32, NewAmmoCount);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnAmmoChanged OnAmmoChanged;

	UFUNCTION(BlueprintCallable, Category="Default")
	void ConsumeAmmo();

	UFUNCTION(BlueprintCallable, Category="Default")
	void AddAmmo(int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Default")
	int32 GetCurrentAmmo() const;

	UFUNCTION(BlueprintCallable, Category="Default")
	bool HasAmmo() const;

private:
	int32 CurrentAmmo = 0;
};
