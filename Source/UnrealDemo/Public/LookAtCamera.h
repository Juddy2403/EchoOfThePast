// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LookAtCamera.generated.h"

UCLASS()
class UNREALDEMO_API UFLookAtCamera : public UObject
{
	GENERATED_BODY()

private:
	static APlayerCameraManager* CachedCameraManager;

public:
	// Makes the given object look at the camera
	UFUNCTION(BlueprintCallable, Category = "Utility")
	static void LookAtCamera(USceneComponent* TargetComponent, bool bLockPitch = false, bool bLockRoll = false);

	// Initialize the cached camera manager
	UFUNCTION(BlueprintCallable, Category = "Utility")
	static void InitializeCameraManager(APlayerController* PlayerController);
};
