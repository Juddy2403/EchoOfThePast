// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PauseHandlerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOOFTHEPAST_API UPauseHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPauseHandlerComponent();

	UFUNCTION(BlueprintCallable, Category="Default")
	void PauseGame(bool IsPaused);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPaused, bool, IsPaused);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnPaused OnPaused;
		
};
