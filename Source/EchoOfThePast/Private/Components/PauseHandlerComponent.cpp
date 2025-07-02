// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/PauseHandlerComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPauseHandlerComponent::UPauseHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UPauseHandlerComponent::PauseGame(bool IsPaused)
{
	OnPaused.Broadcast(IsPaused);
	if (IsPaused)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
		{
			UGameplayStatics::SetGamePaused(GetWorld(), true);
		}));
	}
	else
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}
}

