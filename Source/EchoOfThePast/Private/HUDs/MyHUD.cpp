#include "HUDs/MyHUD.h"

#include "MyBlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/AmmoManagerComponent.h"
#include "Components/ControllerEventsComponent.h"
#include "Components/HealthComponent.h"
#include "Components/LevelManagerComponent.h"
#include "Components/PauseHandlerComponent.h"
#include "Components/TimerComponent.h"
#include "DataTypes/LevelConfigStruct.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/TimerWidgetInterface.h"
#include "Kismet/GameplayStatics.h"

#define TRACE_CursorPlane TraceTypeQuery3

#pragma region Initialization
void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	// Create and add widgets to the viewport
	CreateWidgets(PauseMenuWidgetType, PauseMenuWidget, ESlateVisibility::Collapsed);
	CreateWidgets(MinimapWidgetType, MinimapWidget);
	CreateWidgets(GameHudWidgetType, GameHudWidget);
	CreateWidgets(GameLostWidgetType, GameLostWidget, ESlateVisibility::Collapsed);

	BindEvents();

	// Hide timer if needed
	HandleTimerVisibility();

	if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
	{
		ITimerWidgetInterface::Execute_FadeOut(GameHudWidget);
	}
}

void AMyHUD::CreateWidgets(const TSubclassOf<UUserWidget>& WidgetType, TObjectPtr<UUserWidget>& Widget,
                           const ESlateVisibility& Visibility) const
{
	if (WidgetType)
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetType);
		if (Widget)
		{
			Widget->AddToViewport();
			Widget->SetVisibility(Visibility);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget type is not set!"));
	}
}

void AMyHUD::BindEvents() const
{
	// Binding GameMode events
	if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
	{
		if (UPauseHandlerComponent* PauseHandler = GameMode->GetComponentByClass<UPauseHandlerComponent>())
		{
			PauseHandler->OnPaused.AddDynamic(this, &AMyHUD::OnPaused);
		}
		if (UTimerComponent* TimerComponent = GameMode->GetComponentByClass<UTimerComponent>())
		{
			TimerComponent->OnTimeUp.AddDynamic(this, &AMyHUD::OnGameLost);
			TimerComponent->OnTimeUpdated.AddDynamic(this, &AMyHUD::OnTimeUpdated);
			TimerComponent->On60SecRemaining.AddDynamic(this, &AMyHUD::SetTimerColorRed);
		}
		if (ULevelManagerComponent* LevelManager = GameMode->GetComponentByClass<ULevelManagerComponent>())
		{
			LevelManager->OnOpenLevelTransition.AddDynamic(this, &AMyHUD::FadeIn);
		}
	}
	// Binding Player events
	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (UHealthComponent* HealthComponent = PlayerPawn->FindComponentByClass<UHealthComponent>())
		{
			HealthComponent->OnDeathAfterDelay.AddDynamic(this, &AMyHUD::OnGameLost);
		}
		if (UAmmoManagerComponent* AmmoManager = PlayerPawn->FindComponentByClass<UAmmoManagerComponent>())
		{
			AmmoManager->OnAmmoChanged.AddDynamic(this, &AMyHUD::OnAmmoChanged);
		}
	}
	// Binding Player Controller events
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (UControllerEventsComponent* ControllerEvents = PlayerController->FindComponentByClass<
			UControllerEventsComponent>())
		{
			ControllerEvents->OnPauseMenuCommand.AddDynamic(this, &AMyHUD::SetPausedMenuVisibility);
		}
	}
}
#pragma endregion

void AMyHUD::OnPaused(bool IsPaused)
{
	if (IsPaused)
	{
		if (MinimapWidget) MinimapWidget->SetVisibility(ESlateVisibility::Collapsed);
		if (GameHudWidget) GameHudWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		if (MinimapWidget) MinimapWidget->SetVisibility(ESlateVisibility::Visible);
		if (GameHudWidget) GameHudWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyHUD::SetPausedMenuVisibility(bool IsPaused)
{
	if (!PauseMenuWidget) return;
	ESlateVisibility Visibility = IsPaused ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	PauseMenuWidget->SetVisibility(Visibility);
}

void AMyHUD::OnGameLost()
{
	if (GameLostWidget) GameLostWidget->SetVisibility(ESlateVisibility::Visible);
}

void AMyHUD::SetTimerColorRed()
{
	if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
	{
		ITimerWidgetInterface::Execute_SetTimerColorRed(GameHudWidget);
	}
}

void AMyHUD::OnTimeUpdated(int RemainingTime, FText RemainingTimeText)
{
	if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
	{
		ITimerWidgetInterface::Execute_UpdateTimerText(GameHudWidget, RemainingTimeText);
	}
}

void AMyHUD::HandleTimerVisibility() const
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	FSLevelConfigStruct* LevelConfig = LevelConfigTable->FindRow<FSLevelConfigStruct>(
		*LevelName, TEXT("Get Level Config Row"));

	if (!LevelConfig || LevelConfig->HasTimer) return;
	if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
	{
		ITimerWidgetInterface::Execute_HideTimer(GameHudWidget);
	}
}

void AMyHUD::OnAmmoChanged(int32 AmmoCount)
{
	if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
	{
		ITimerWidgetInterface::Execute_SetAmmoCount(GameHudWidget, AmmoCount);
	}
}

void AMyHUD::FadeIn()
{
	if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
	{
		ITimerWidgetInterface::Execute_FadeIn(GameHudWidget);
	}
}


void AMyHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetCursorPosition();
}

void AMyHUD::SetCursorPosition() const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController) return;
	FVector PosToProject = FVector::ZeroVector;
	bool hit = UMyBlueprintFunctionLibrary::GetCursorPosition(this, PosToProject);
	if (!hit) return;
	FVector2D ScreenPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		PlayerController, PosToProject, ScreenPosition, false);
	// Finally, giving the position to the widget
	if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
	{
		ITimerWidgetInterface::Execute_SetCursorPosition(GameHudWidget, ScreenPosition);
	}
}
