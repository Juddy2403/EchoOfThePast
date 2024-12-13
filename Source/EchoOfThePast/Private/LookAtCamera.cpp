#include "LookAtCamera.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/PlayerCameraManager.h"

TObjectPtr<APlayerCameraManager> UFLookAtCamera::CachedCameraManager = nullptr;

void UFLookAtCamera::LookAtCamera(USceneComponent* TargetComponent, bool bLockPitch, bool bLockRoll)
{
	if (!TargetComponent || !CachedCameraManager) return;

	// Get camera location
	const FVector CameraLocation = CachedCameraManager->GetCameraLocation();

	// Get target location
	const FVector TargetLocation = TargetComponent->GetComponentLocation();

	// Find the rotation needed for the target to face the camera
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(TargetLocation, CameraLocation);

	// Optionally lock certain axes
	if (bLockPitch)
	{
		LookAtRotation.Pitch = TargetComponent->GetComponentRotation().Pitch;
	}
	if (bLockRoll)
	{
		LookAtRotation.Roll = TargetComponent->GetComponentRotation().Roll;
	}

	// Apply the rotation
	TargetComponent->SetWorldRotation(LookAtRotation);
}

void UFLookAtCamera::InitializeCameraManager(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		CachedCameraManager = PlayerController->PlayerCameraManager;
	}
}
