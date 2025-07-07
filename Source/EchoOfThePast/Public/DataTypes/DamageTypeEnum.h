// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EDamageTypeEnum : uint8
{
	Melee      UMETA(DisplayName = "Melee"),
	Ranged     UMETA(DisplayName = "Ranged"),
	None       UMETA(DisplayName = "None"),
};