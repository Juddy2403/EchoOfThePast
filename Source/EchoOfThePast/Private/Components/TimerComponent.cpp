// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TimerComponent.h"

// Sets default values for this component's properties
UTimerComponent::UTimerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 1.f;
	// ...
}

FText UTimerComponent::GetRemainingTime() const
{
	return FText::Format(
		FText::FromString("{0}:{1}"),
		FText::AsNumber(RemainingTime / 60, &FNumberFormattingOptions().SetMinimumIntegralDigits(2)),
		FText::AsNumber(RemainingTime % 60, &FNumberFormattingOptions().SetMinimumIntegralDigits(2))
	);
}

// Called when the game starts
void UTimerComponent::BeginPlay()
{
	Super::BeginPlay();
	RemainingTime = TimerDuration;
}

// Called every frame
void UTimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (RemainingTime > 0)
	{
		--RemainingTime;
		OnTimeUpdated.Broadcast(RemainingTime, GetRemainingTime());
		if (RemainingTime == 60)
		{
			On60SecRemaining.Broadcast();
		}
	}
	else
	{
		OnTimeUp.Broadcast();
		SetComponentTickEnabled(false);
	}
}
