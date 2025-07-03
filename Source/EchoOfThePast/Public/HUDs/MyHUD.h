// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class ECHOOFTHEPAST_API AMyHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> PauseMenuWidgetType;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> GameHudWidgetType;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> GameLostWidgetType;
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UUserWidget> MinimapWidgetType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> LevelConfigTable;
	
private:
	UPROPERTY()
	TObjectPtr<UUserWidget> PauseMenuWidget;
	UPROPERTY()
	TObjectPtr<UUserWidget> GameHudWidget;
	UPROPERTY()
	TObjectPtr<UUserWidget> GameLostWidget;
	UPROPERTY()
	TObjectPtr<UUserWidget> MinimapWidget;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	UFUNCTION()
	void HandleTimerVisibility() const;
	UFUNCTION()
	void SetCursorPosition() const;
	UFUNCTION()
	void OnAmmoChanged(int32 AmmoCount);
	UFUNCTION()
	void FadeIn();
	UFUNCTION()
	void OnPaused(bool IsPaused);
	UFUNCTION()
	void SetPausedMenuVisibility(bool IsPaused);
	UFUNCTION()
	void OnGameLost();
	UFUNCTION()
	void SetTimerColorRed();
	UFUNCTION()
	void OnTimeUpdated(int RemainingTime, FText RemainingTimeText);
	void CreateWidgets(const TSubclassOf<UUserWidget>& WidgetType, TObjectPtr<UUserWidget>& Widget,
		const ESlateVisibility& Visibility = ESlateVisibility::Visible) const;
	void BindEvents() const;
};
