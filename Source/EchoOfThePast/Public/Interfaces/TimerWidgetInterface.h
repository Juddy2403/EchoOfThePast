// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TimerWidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTimerWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECHOOFTHEPAST_API ITimerWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Timer")
	void HideTimer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Timer")
	void UpdateTimerText(const FText& TimerText);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Timer")
	void SetTimerColorRed();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ammo")
	void SetAmmoCount(const int32& AmmoCount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Cursor")
	void SetCursorPosition(const FVector2D& ScreenPosition);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fade")
	void FadeIn();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Fade")
	void FadeOut();
};
