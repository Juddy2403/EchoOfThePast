// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataStructs/PlayerStatsStruct.h"
#include "Engine/GameInstance.h"
#include "Interfaces/GameStatsInterface.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ECHOOFTHEPAST_API UMyGameInstance : public UGameInstance, public IGameStatsInterface
{
	GENERATED_BODY()
public:
	virtual void SaveGameStats_Implementation() override;
	virtual void LoadGameStats_Implementation() override;
private:
	bool bHasDataToLoad = false;
	FPlayerStatsStruct PlayerStats;
	int RemainingTime = 0;
};
