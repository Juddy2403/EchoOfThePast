# Echo of the Past

Echo of the Past is my Unreal-powered action project where I glued together my favorite gameplay loops: quick weapon
swaps, punchy melee combos, and a dialogue system. It’s my first and biggest yet Unreal indie project.

For more information on my design and gameplay mechancis, check the project out [here](https://juddy2403.github.io/echo-of-the-past.html).
This is a dev-focused README covering the C++ systems and architecture.

To play the game, download the [latest Windows build](https://github.com/Juddy2403/EchoOfThePast/releases/tag/V3.5).

---

## TL;DR

- Engine: Unreal Engine (C++ with Blueprint support)
- Core systems: GameMode components (Pause, Timer, Level transitions), HUD bound via delegates, Player combat (
  melee/ranged with combos and pickups), Controller events 
- Design vibe: Event-driven UI, componentized gameplay, data-driven tuning
- Platforms: Windows (Editor/Packaged)

---

## Build & run source project

- Open the project in Unreal Editor (5.4.4 recommended).
- Ensure the startup map and game mode are set (they’re configured in Config/DefaultEngine.ini).
- Press Play in Editor or package as usual.

Relevant config (Config/DefaultEngine.ini):

```ini
GlobalDefaultGameMode = /Game/Blueprints/GameModes/BP_MyGameMode.BP_MyGameMode_C
EditorStartupMap = /Game/Levels/L_Level0_1.L_Level0_1
GameInstanceClass = /Script/EchoOfThePast.MyGameInstance
```

---

## C++ systems

- GameMode components own global game state:
    - PauseHandlerComponent — exposes OnPaused(bool)
    - TimerComponent — counts down, exposes OnTimeUp, OnTimeUpdated, On60SecRemaining
    - LevelManagerComponent — raises OnOpenLevelTransition on transitions
- Player controller & pawn emit gameplay signals:
    - ControllerEventsComponent — multicast pause menu toggle
    - Ammo/Health components — ammo updates, death-with-delay, etc.
- HUD wires everything together:
    - Subscribes to GameMode + Player + Controller delegates
    - Shows/hides widgets (HUD, minimap, pause menu, game lost)
    - Pushes UX affordances (e.g., turn the clock red at 60s)

This keeps UI reactive and decoupled. Gameplay raises signals; HUD listens and updates.

---

## Input: Controller events (pause workflow)

The controller exposes a blueprint-assignable multicast delegate for pause toggling. It’s small and surgical—no
device-specific details leak into UI logic.

Header (Source/EchoOfThePast/Public/Components/ControllerEventsComponent.h):

```c++
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ECHOOFTHEPAST_API UControllerEventsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UControllerEventsComponent();

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseMenuCommand, bool, bIsPaused);

    UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Events")
    FOnPauseMenuCommand OnPauseMenuCommand;
};
```

[Open file](https://github.com/Juddy2403/EchoOfThePast/blob/ee09bf301802a8d440a6c21e71be60f8eb500014/Source/EchoOfThePast/Public/Components/ControllerEventsComponent.h)

HUD wiring (Source/EchoOfThePast/Private/HUDs/MyHUD.cpp):

```c++
if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
{
    if (UControllerEventsComponent* ControllerEvents = PC->FindComponentByClass<UControllerEventsComponent>())
    {
        ControllerEvents->OnPauseMenuCommand.AddDynamic(this, &AMyHUD::SetPausedMenuVisibility);
    }
}
```

And the actual UI reaction is tiny and explicit:

```c++
void AMyHUD::SetPausedMenuVisibility(bool IsPaused)
{
    if (!PauseMenuWidget) return;
    const ESlateVisibility Visibility = IsPaused ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
    PauseMenuWidget->SetVisibility(Visibility);
}
```

[Open file](https://github.com/Juddy2403/EchoOfThePast/blob/ee09bf301802a8d440a6c21e71be60f8eb500014/Source/EchoOfThePast/Private/HUDs/MyHUD.cpp#L62-L143)

---

## HUD: event-driven UI

My HUD subscribes to exactly what it cares about. The code reads like a checklist:

- Game paused? Hide minimap and in-game HUD.
- Time updated? Update UI text.
- 60s remaining? Turn the timer red.
- Player died? Show “Game Lost”.
- Level transition? Fade-in.

Binding section (Source/EchoOfThePast/Private/HUDs/MyHUD.cpp):

```c++
// GameMode events
if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this)) {
    if (UPauseHandlerComponent* Pause = GM->GetComponentByClass<UPauseHandlerComponent>())
        Pause->OnPaused.AddDynamic(this, &AMyHUD::OnPaused);

    if (UTimerComponent* Timer = GM->GetComponentByClass<UTimerComponent>()) {
        Timer->OnTimeUp.AddDynamic(this, &AMyHUD::OnGameLost);
        Timer->OnTimeUpdated.AddDynamic(this, &AMyHUD::OnTimeUpdated);
        Timer->On60SecRemaining.AddDynamic(this, &AMyHUD::SetTimerColorRed);
    }
    if (ULevelManagerComponent* LM = GM->GetComponentByClass<ULevelManagerComponent>())
        LM->OnOpenLevelTransition.AddDynamic(this, &AMyHUD::FadeIn);
}

// Player events
if (APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0)) {
    if (UHealthComponent* Health = Pawn->FindComponentByClass<UHealthComponent>())
        Health->OnDeathAfterDelay.AddDynamic(this, &AMyHUD::OnGameLost);

    if (UAmmoManagerComponent* Ammo = Pawn->FindComponentByClass<UAmmoManagerComponent>())
        Ammo->OnAmmoChanged.AddDynamic(this, &AMyHUD::OnAmmoChanged);
}
```

The handlers themselves are dead simple:

```c++
void AMyHUD::OnPaused(bool IsPaused)
{
    const auto Vis = IsPaused ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
    if (MinimapWidget) MinimapWidget->SetVisibility(Vis);
    if (GameHudWidget) GameHudWidget->SetVisibility(Vis);
}

void AMyHUD::OnTimeUpdated(int RemainingTime, FText RemainingText)
{
    if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
        ITimerWidgetInterface::Execute_UpdateTimerText(GameHudWidget, RemainingText);
}
```

[Open file](https://github.com/Juddy2403/EchoOfThePast/blob/ee09bf301802a8d440a6c21e71be60f8eb500014/Source/EchoOfThePast/Private/HUDs/MyHUD.cpp#L62-L143)

---

## Combat: weapon swaps and melee combos

Combat is handled by a component that equips weapons (melee/ranged), manages combo windows, and spawns pickups when
dropping old gear. It’s gameplay-lean and intentionally readable.

Weapon swapping (Source/EchoOfThePast/Private/Components/AttackManagerComponent.cpp):

```c++
void UAttackManagerComponent::SwitchAttackType(const EDamageTypeEnum Type)
{
    switch (Type)
    {
    case EDamageTypeEnum::Melee:   SwitchWeapon(EquippedSword, true); break;
    case EDamageTypeEnum::Ranged:  SwitchWeapon(EquippedGun); break;
    default: break;
    }
}

void UAttackManagerComponent::SwitchWeapon(const TSubclassOf<ABaseWeapon>& WeaponClass, bool bIsSwitchingToMelee)
{
    if (bIsSwitchingToMelee && !IsValid(EquippedSword)) {
        if (IsValid(CurrentWeapon)) CurrentWeapon->Destroy();
        return;
    }
    if (!IsValid(WeaponClass)) return;
    if (IsValid(CurrentWeapon)) CurrentWeapon->Destroy();

    FActorSpawnParameters Params; Params.Owner = OwnerCharacter;
    CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, FTransform::Identity, Params);
    CurrentWeapon->AttachToComponent(
        OwnerCharacter->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        CurrentWeapon->SocketName
    );
}
```

Combo pacing (same file):

```c++
UAnimMontage* UAttackManagerComponent::GetMeleeMontage(const float& WeaponPlayRate)
{
    const float Now = UGameplayStatics::GetTimeSeconds(GetWorld());
    const float Interval = (1.f / WeaponPlayRate) * 2.f; // simple anti-mash window

    if (CurrentCombo >= AttackMontages.Num()) CurrentCombo = -1;
    if ((Now - LastAttackTime) > Interval)     CurrentCombo = -1;

    ++CurrentCombo;
    LastAttackTime = Now;

    ComboModifier = CurrentCombo > PowerfulComboHigherThan ? 1.5f : 1.f;
    return AttackMontages.IsValidIndex(CurrentCombo) ? AttackMontages[CurrentCombo] : nullptr;
}
```

[Open file](https://github.com/Juddy2403/EchoOfThePast/blob/ee09bf301802a8d440a6c21e71be60f8eb500014/Source/EchoOfThePast/Private/Components/AttackManagerComponent.cpp#L49-L136)

---

## Timing and stakes

A central timer keeps pressure on the player. It drives:

- “Time’s up” loss state (OnTimeUp)
- Minute-remaining visual cue (On60SecRemaining)
- Continuous UI updates (OnTimeUpdated)

You’ll see the HUD flip the clock red right on cue:

```c++
void AMyHUD::SetTimerColorRed()
{
    if (GameHudWidget->GetClass()->ImplementsInterface(UTimerWidgetInterface::StaticClass()))
        ITimerWidgetInterface::Execute_SetTimerColorRed(GameHudWidget);
}
```

---

## Rendering, audio, and platform notes

- Rendering targets DX12 by default (configurable in DefaultEngine.ini).
- Audio: project-level settings (sample rate, callback size, worker count) are tuned in config. Gameplay audio hooks
  live with their owning systems/components and fire off from events (e.g., on hit, on swap, on lose).
- Platform defaults are conservative (no Nanite, no virtual textures) to keep the Editor snappy.

---

## Design notes (why it’s built this way)

- Event-first UI: HUD doesn’t poll; it listens—less boilerplate, fewer desync bugs.
- Components own behavior: attack logic in an AttackManagerComponent, not scattered across the pawn or animation BP.
- Slim bridges: Controller events and GameMode components translate engine-level happenings into game-level signals you
  can unit test at the C++ layer.
- Data turns the knobs: timing windows, combo thresholds, and widgets are configured, not hard-coded into UI.

---

## Contributing / hacking

- Want a new HUD reaction? Add a delegate and bind it—no monolith needed.
- New weapon? Make a class, wire its pickup class, drop the old one via `DropOldWeapon`, and let the manager attach it
  to the right socket.
- Tweak pressure? Adjust `TimerComponent` thresholds and the UI will follow.



