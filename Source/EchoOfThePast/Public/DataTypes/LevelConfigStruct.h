// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "LevelConfigStruct.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FSLevelConfigStruct : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="NextLevelName", MakeStructureDefaultValue="None"))
	FName NextLevelName = NAME_None;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="ShouldKeepPlayerStats", MakeStructureDefaultValue="False"))
	bool ShouldKeepPlayerStats = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="HPModifier", MakeStructureDefaultValue="0.000000"))
	double HPModifier = 0.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="SpeedModifier", MakeStructureDefaultValue="0.000000"))
	double SpeedModifier = 0.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="DamageModifier", MakeStructureDefaultValue="0.000000"))
	double DamageModifier = 0.0;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="CombatEnabled", MakeStructureDefaultValue="False"))
	bool CombatEnabled = false;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="HasTimer", MakeStructureDefaultValue="False"))
	bool HasTimer = false;
};
