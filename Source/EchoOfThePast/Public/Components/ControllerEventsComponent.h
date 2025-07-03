// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ControllerEventsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOOFTHEPAST_API UControllerEventsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UControllerEventsComponent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseMenuCommand, bool, bIsPaused);
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events")
	FOnPauseMenuCommand OnPauseMenuCommand;

};
