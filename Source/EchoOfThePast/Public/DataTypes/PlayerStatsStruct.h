#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerStatsStruct.generated.h"
class ABaseMeleeWeapon;
class ABaseRangedWeapon;

USTRUCT(BlueprintType)
struct FPlayerStatsStruct
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Health = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Ammo = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseRangedWeapon> EquippedGun = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseMeleeWeapon> EquippedSword = nullptr;
};
