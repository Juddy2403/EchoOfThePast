#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStatsStruct.generated.h"
class ABaseWeapon;

USTRUCT(BlueprintType)
struct FPlayerStatsStruct
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Ammo = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseWeapon> EquippedGun = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseWeapon> EquippedSword = nullptr;
};
