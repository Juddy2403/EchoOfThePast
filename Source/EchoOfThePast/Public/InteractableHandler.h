// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECHOOFTHEPAST_API IInteractableHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction") 
	void AddInteractable(const TScriptInterface<class UInteractable>& Interactable);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction") 
	void RemoveInteractable(const TScriptInterface<class UInteractable>& Interactable); 
};
