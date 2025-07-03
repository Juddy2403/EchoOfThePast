// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LevelManagerComponent.h"

#include "Interfaces/GameStatsInterface.h"
#include "DataTypes/LevelConfigStruct.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULevelManagerComponent::ULevelManagerComponent(): LevelConfigTable(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void ULevelManagerComponent::OpenLevelWithTransition(const FString& LevelName)
{
	OnOpenLevelTransition.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(
		DelayHandle,
		FTimerDelegate::CreateLambda([this, LevelName]()
		{
			UGameplayStatics::OpenLevel(this, static_cast<FName>(LevelName));
			OnLevelOpened.Broadcast();
		}),
		1.0f,
		false
	);
}

void ULevelManagerComponent::ReloadLevel()
{
	FString Name = UGameplayStatics::GetCurrentLevelName(GetWorld());
	FName CurrentLevelName(*Name, 0);

	if (CurrentLevelName.ToString().Contains("L_Level2"))
	{
		// Level 2 is split into multiple parts; reset always goes back to the first part
		UGameplayStatics::OpenLevel(this, FName("L_Level2_1"));
	}
	else UGameplayStatics::OpenLevel(this, static_cast<FName>(CurrentLevelName));
	OnLevelOpened.Broadcast();
}

void ULevelManagerComponent::LoadNextLevel()
{
	FString Name = UGameplayStatics::GetCurrentLevelName(GetWorld());

	FSLevelConfigStruct* LevelConfig = LevelConfigTable->FindRow<FSLevelConfigStruct>(
		*Name, TEXT("Get Level Config Row"));

	if (!LevelConfig) return;
	if (LevelConfig->ShouldKeepPlayerStats)
	{
		UGameInstance* GameInstance = GetWorld()->GetGameInstance();
		if (GameInstance->GetClass()->ImplementsInterface(UGameStatsInterface::StaticClass()))
		{
			IGameStatsInterface::Execute_SaveGameStats(GameInstance);
		}
	}
	OpenLevelWithTransition(LevelConfig->NextLevelName.ToString());
	OnLevelOpened.Broadcast();
}

// Called when the game starts
void ULevelManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}
