// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes/PlayerStatsStruct.h"
#include "Engine/GameInstance.h"
#include "Interfaces/GameStatsInterface.h"
#include "Interfaces/MusicHandlerInterface.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ECHOOFTHEPAST_API UMyGameInstance : public UGameInstance, public IGameStatsInterface, public IMusicHandlerInterface
{
	GENERATED_BODY()
public:
	virtual void SaveGameStats_Implementation() override;
	virtual void LoadGameStats_Implementation() override;
	virtual void EnableMusic_Implementation(bool bEnable) override;
	virtual void SetupMusic_Implementation(USoundBase* MusicToPlay, float VolumeMultiplier = 1.f) override;
	virtual bool IsMusicEnabled_Implementation() const override { return bIsMusicEnabled; }
private:
	UPROPERTY()
	bool bHasDataToLoad = false;
	UPROPERTY()
	FPlayerStatsStruct PlayerStats;
	UPROPERTY()
	int RemainingTime = 0;
	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;
	UPROPERTY()
	bool bIsMusicEnabled = true;
	UPROPERTY()
	bool bIsCombatEnabled = false;
};
