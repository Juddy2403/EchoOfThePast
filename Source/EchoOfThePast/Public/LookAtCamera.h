#pragma once

#include "CoreMinimal.h"
#include "LookAtCamera.generated.h"

UCLASS()
class ECHOOFTHEPAST_API UFLookAtCamera : public UObject
{
	GENERATED_BODY()
private:
	static TObjectPtr<APlayerCameraManager> CachedCameraManager;
public:
	// Makes the given object look at the camera
	UFUNCTION(BlueprintCallable, Category = "Utility")
	static void LookAtCamera(USceneComponent* TargetComponent, bool bLockPitch = false, bool bLockRoll = false);

	// Initialize the cached camera manager
	UFUNCTION(BlueprintCallable, Category = "Utility")
	static void InitializeCameraManager(APlayerController* PlayerController);
};
