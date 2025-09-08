// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MusicHandlerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMusicHandlerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ECHOOFTHEPAST_API IMusicHandlerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EnableMusic(bool bEnable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetupMusic(USoundBase* MusicToPlay, float VolumeMultiplier = 1.f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsMusicEnabled() const;
};
