// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LevelManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOOFTHEPAST_API ULevelManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULevelManagerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UDataTable* LevelConfigTable;

	UFUNCTION(BlueprintCallable, Category="Default")
	void OpenLevelWithTransition(const FString& LevelName);

	UFUNCTION(BlueprintCallable, Category="Default")
	void ReloadLevel();

	UFUNCTION(BlueprintCallable, Category="Default")
	void LoadNextLevel();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelOpened);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnLevelOpened OnLevelOpened;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenLevelTransition);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FOnOpenLevelTransition OnOpenLevelTransition;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FTimerHandle DelayHandle;
};
