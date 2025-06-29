// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerModifiersInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerModifiersInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECHOOFTHEPAST_API IPlayerModifiersInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) 
	void SetPlayerModifiers(const float& HealthModifier, const float& SpeedModifier, const float& DamageModifier);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) 
	void GetPlayerModifiers(float& HealthModifier, float& SpeedModifier, float& DamageModifier); 
};
