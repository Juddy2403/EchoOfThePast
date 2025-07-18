// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/PlayerStatsStruct.h"
#include "UObject/Interface.h"
#include "PlayerStatsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerStatsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECHOOFTHEPAST_API IPlayerStatsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) 
	void SetPlayerStats(const FPlayerStatsStruct& Stats);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent) 
	FPlayerStatsStruct GetPlayerStats();
};
