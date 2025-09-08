// Fill out your copyright notice in the Description page of Project Settings.
#include "GameInstances/MyGameInstance.h"

#include "Components/AudioComponent.h"
#include "Components/LevelManagerComponent.h"
#include "Components/TimerComponent.h"
#include "DataTypes/LevelConfigStruct.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/PlayerStatsInterface.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::SaveGameStats_Implementation()
{
	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn->GetClass()->ImplementsInterface(UPlayerStatsInterface::StaticClass()))
	{
		bHasDataToLoad = true;
		PlayerStats = IPlayerStatsInterface::Execute_GetPlayerStats(PlayerPawn);
	}
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (UTimerComponent* TimerComponent = GameMode->GetComponentByClass<UTimerComponent>())
	{
		RemainingTime = TimerComponent->RemainingTime;
		bHasDataToLoad = true;
	}
}

void UMyGameInstance::LoadGameStats_Implementation()
{
	if (!bHasDataToLoad) return;
	bHasDataToLoad = false;
	// delay till next tick
	AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PlayerPawn->GetClass()->ImplementsInterface(UPlayerStatsInterface::StaticClass()))
	{
		IPlayerStatsInterface::Execute_SetPlayerStats(PlayerPawn, PlayerStats);
	}
	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	if (UTimerComponent* TimerComponent = GameMode->GetComponentByClass<UTimerComponent>())
	{
		TimerComponent->RemainingTime = RemainingTime;
		TimerComponent->OnTimeUpdated.Broadcast(TimerComponent->RemainingTime, TimerComponent->GetRemainingTime());
	}
}

void UMyGameInstance::EnableMusic_Implementation(bool bEnable)
{
	bIsMusicEnabled = bEnable;
	if (!IsValid(AudioComponent) || !bIsCombatEnabled) return;
	AudioComponent->SetPaused(!bEnable);
}

void UMyGameInstance::SetupMusic_Implementation(USoundBase* MusicToPlay, float VolumeMultiplier)
{
	FSLevelConfigStruct* LevelConfig = UGameplayStatics::GetGameMode(GetWorld())->
	                                   GetComponentByClass<ULevelManagerComponent>()->GetCurrentLevelConfig();
	bIsCombatEnabled = LevelConfig->CombatEnabled;
	if (!IsValid(AudioComponent))
	{
		AudioComponent = UGameplayStatics::SpawnSound2D(this, MusicToPlay, VolumeMultiplier,
			1.f, 0.f, nullptr, true, false);
	}
	if (bIsCombatEnabled && bIsMusicEnabled)
	{
		AudioComponent->SetPaused(false);
	}
	if (!bIsCombatEnabled)
	{
		AudioComponent->SetPaused(true);
	}
}
