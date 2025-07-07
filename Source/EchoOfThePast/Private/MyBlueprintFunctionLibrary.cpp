// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBlueprintFunctionLibrary.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#define TRACE_CursorPlane TraceTypeQuery3

uint64 UMyBlueprintFunctionLibrary::LastFrame = 0;
FVector UMyBlueprintFunctionLibrary::CachedMousePosition = FVector::ZeroVector;

bool UMyBlueprintFunctionLibrary::GetCursorPosition(const UObject* Object, FVector& MousePosition)
{
	MousePosition = CachedMousePosition;
	if (LastFrame == GFrameCounter) return true;
	LastFrame = GFrameCounter;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(Object->GetWorld(), 0);
	if (!PlayerController) return false;
	FHitResult HitResult;
	// Trace custom channel to detect cursor only on player plane
	bool bHit = PlayerController->
		GetHitResultUnderCursorByChannel(ETraceTypeQuery::TRACE_CursorPlane, false, HitResult);
	if (!bHit || !HitResult.bBlockingHit) return false;
	FVector PosToProject = HitResult.Location;
	// TODO:kinda hard coded and bad
	PosToProject.Z = UGameplayStatics::GetPlayerCharacter(
		Object->GetWorld(), 0)->GetMesh()->GetComponentLocation().Z + 110.f;
	MousePosition = PosToProject;
	CachedMousePosition = MousePosition;
	return true;
}
