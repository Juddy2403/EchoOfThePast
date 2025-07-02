// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOOFTHEPAST_API UTimerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimerComponent();

	UFUNCTION(BlueprintCallable, Category = "Default")
	FText GetRemainingTime() const;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	int TimerDuration = 60;
	UPROPERTY(BlueprintReadOnly, Category = "Default")
	int RemainingTime = 0;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeUp);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnTimeUp OnTimeUp;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimeUpdated, int, RemainingTime, FText, RemainingTimeText);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnTimeUpdated OnTimeUpdated;
	
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOn60SecRemaining);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOn60SecRemaining On60SecRemaining;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
