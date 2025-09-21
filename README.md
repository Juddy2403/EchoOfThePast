# Echo of the Past 🎮

> *An Unreal Engine 5 action RPG where androids travel through time to save humanity from itself*

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.4.4-313131?style=flat&logo=unrealengine)](https://www.unrealengine.com/)
[![C++](https://img.shields.io/badge/C++-91.5%25-00599C?style=flat&logo=c%2B%2B)](https://isocpp.org/)
[![Python](https://img.shields.io/badge/Python-5.6%25-3776AB?style=flat&logo=python)](https://www.python.org/)
[![Latest Release](https://img.shields.io/github/v/release/Juddy2403/EchoOfThePast)](https://github.com/Juddy2403/EchoOfThePast/releases/latest)

Echo of the Past started as a class assignment to learn Unreal Engine basics, but I fell in love with the engine and spent months turning it into my biggest indie project yet. What began with keywords "future" and "resource management" evolved into a combat RPG where the last remaining androids must travel back in time to prevent humanity's self-destruction.

**[🎮 Download Latest Build](https://github.com/Juddy2403/EchoOfThePast/releases/latest) | [📖 Full Project Details](https://juddy2403.github.io/echo-of-the-past.html) | [🎯 Play in Browser](https://itch.io/link-when-available)**

[![Watch the video](https://i9.ytimg.com/vi/tQl6Na0fBCw/maxresdefault.jpg?v=68c020a3&sqp=CMz8wMYG&rs=AOn4CLBBnaTmirQ5QhdSXXry0hxPVrwTEw)](https://youtu.be/tQl6Na0fBCw?si=sjUxrBfX2mgocXYE)
---

## 🚀 Quick Start

### Prerequisites
- **Unreal Engine 5.4.4** (recommended)
- **Windows 10/11** (packaged builds)
- **Visual Studio 2022** with C++ workload (source builds)

### Running the Game
1. **Download & Play**: Get the [latest Windows build](https://github.com/Juddy2403/EchoOfThePast/releases/latest) and run `EchoOfThePast.exe`
2. **From Source**: Clone this repo, open `EchoOfThePast.uproject` in UE 5.4.4, and hit Play

```bash
git clone https://github.com/Juddy2403/EchoOfThePast.git
cd EchoOfThePast
# Open EchoOfThePast.uproject in Unreal Editor
```

---

## ⚔️ What Makes This Special

Echo of the Past showcases **complex, interlocking gameplay systems** built with C++ and Blueprint integration. Every feature was designed with **performance and maintainability** in mind.

### 🎯 Core Combat Features
- **Dual Combat Modes**: Seamless melee/ranged switching with different mechanics for each
- **Dynamic Combo System**: Timing-based combos with critical hit finishers
- **16 Unique Weapons**: 8 melee + 8 ranged with distinct stats and behaviors
- **Smart Weapon Pickups**: Visual stat comparison when hovering over better gear

### 🤖 Advanced AI Systems
- **Behavior Tree Intelligence**: Modular AI that adapts based on player actions
- **Multi-Sensory Detection**: Enemies see, hear footsteps, gunshots, and sword attacks
- **Dynamic Combat Roles**: Same AI framework handles both melee and ranged enemies
- **Stagger Mechanics**: Timing-based vulnerability windows for tactical gameplay

### 🔧 Technical Solutions I'm Proud Of

**3D Aiming in a Crosscode-Inspired System**: Solved cursor accuracy in 3D by creating custom trace channels hitting an invisible plane through the player's center—no more missed shots due to perspective issues.

**Custom Occlusion System**: Built my own character visibility solution after third-party plugins caused crashes. Material functions set opacity masks to 0 when objects block the camera, with a Python script automatically applying this to all materials.

**Event-Driven UI Architecture**: HUD subscribes to GameMode and Player delegates for reactive updates. No polling, no desync—just clean separation between gameplay and presentation.

---

## 🏗️ Architecture Deep Dive

The codebase follows **component-driven design** with event-based communication. Here's how the major systems work together:

### Component Architecture
```
GameMode Components (Global State):
├── PauseHandlerComponent     → OnPaused(bool)
├── TimerComponent           → OnTimeUp, OnTimeUpdated, On60SecRemaining  
└── LevelManagerComponent    → OnOpenLevelTransition

Player Systems:
├── AttackManagerComponent   → Combat logic, weapon swapping, combos
├── HealthComponent         → OnDeathAfterDelay  
├── AmmoManagerComponent    → OnAmmoChanged
└── ControllerEventsComponent → OnPauseMenuCommand
```

### Key Implementation Highlights

**Weapon Swapping System** ([View Code](Source/EchoOfThePast/Private/Components/AttackManagerComponent.cpp#L45))
```cpp
void UAttackManagerComponent::SwitchWeapon(const TSubclassOf<ABaseWeapon>& WeaponClass, bool bIsSwitchingToMelee)
{
    if (IsValid(CurrentWeapon)) CurrentWeapon->Destroy();
    
    FActorSpawnParameters Params; 
    Params.Owner = OwnerCharacter;
    CurrentWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, FTransform::Identity, Params);
    CurrentWeapon->AttachToComponent(
        OwnerCharacter->GetMesh(),
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        CurrentWeapon->SocketName
    );
}
```

**Event-Driven HUD Updates** ([View Code](Source/EchoOfThePast/Private/HUDs/MyHUD.cpp#L89))
```cpp
void AMyHUD::OnPaused(bool IsPaused)
{
    const auto Vis = IsPaused ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
    if (MinimapWidget) MinimapWidget->SetVisibility(Vis);
    if (GameHudWidget) GameHudWidget->SetVisibility(Vis);
}
```

---

## 🎨 Behind the Scenes

### The Story That Drives Everything
Four androids remain on a barren Earth—humanity's last consciousness. Using the final resources, they build an unstable time portal. Only one can go back to key moments in human history, slowing down the evolution that led to destruction. But with each mission, you grow weaker... after all, you are humanity's **greatest creation**.

### Technical Challenges Solved
- **Cross-Code Integration**: Seamless Blueprint/C++ workflow for rapid iteration
- **Performance Optimization**: Component-based architecture eliminates unnecessary updates
- **Shader Programming**: Custom materials for melee radius visualization and occlusion
- **Python Automation**: Material batch processing for consistent occlusion behavior
- **Niagara VFX Integration**: Damage numbers with physics-based randomization

---

## 🛠️ Tech Stack

| **Category** | **Technologies** |
|--------------|------------------|
| **Engine** | Unreal Engine 5.4.4 |
| **Programming** | C++ (91.5%), Python (5.6%), C# (2.3%) |
| **Graphics** | DX12, Custom Shaders, Niagara VFX |
| **AI** | Behavior Trees, Pawn Sensing, Blackboards |
| **Audio** | Spatial Audio, AI Voice-overs, Dynamic SFX |
| **Tools** | Python Material Automation, Custom Editor Tools |

---

## 🚀 Installation & Development

### For Players
1. Download the [latest release](https://github.com/Juddy2403/EchoOfThePast/releases/latest)
2. Extract and run `EchoOfThePast.exe`
3. Enjoy the time-traveling android adventure!

### For Developers
```bash
# Clone the repository
git clone https://github.com/Juddy2403/EchoOfThePast.git
cd EchoOfThePast

# Open in Unreal Editor (requires UE 5.4.4)
# Double-click EchoOfThePast.uproject

# Or generate Visual Studio files
"C:\Program Files\Epic Games\UE_5.4\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="EchoOfThePast.uproject" -game -rocket -progress
```

### Configuration
Key settings are in `Config/DefaultEngine.ini`:
```ini
[/Script/EngineSettings.GameMapsSettings]
GlobalDefaultGameMode=/Game/Blueprints/GameModes/BP_MyGameMode.BP_MyGameMode_C
EditorStartupMap=/Game/Levels/L_Level0_1.L_Level0_1
GameInstanceClass=/Script/EchoOfThePast.MyGameInstance
```

---

## 🎯 Features Showcase

<details>
<summary><strong>🗺️ Combat System Details</strong></summary>

**Melee Combat**:
- Combo chains with timing windows
- 360° attack radius visualization
- Stagger mechanics for tactical depth
- Critical hit system on combo finishers

**Ranged Combat**:
- Custom 3D cursor placement on invisible plane
- Multiple projectile types (hitscan, projectile, explosive)
- Ammo management with pickup system
- Laser sight telegraphing for enemy attacks

**Weapon Variety**:
- 8 melee weapons with different speeds and crit rates
- 8 ranged weapons across 3 projectile categories
- Visual stat comparison on ground pickups
- Shared weapon classes between player and enemies

</details>

<details>
<summary><strong>🤖 AI Behavior System</strong></summary>

**Detection Systems**:
- Visual detection with line-of-sight checking
- Audio detection (footsteps, weapon sounds, combat)
- Forgetting mechanism after losing player contact
- Blackboard-driven state management

**Combat Behaviors**:
- Melee enemies: Close distance, face player, combo attacks
- Ranged enemies: Maintain distance, laser telegraphing, burst fire
- Dynamic movement speed based on player distance
- Stagger vulnerability windows during attack preparation

**Customization**:
- Editor-exposed weapon and health settings
- Modular Behavior Tree for easy enemy variants
- Shared weapon system with player (minus ammo for ranged)

</details>

<details>
<summary><strong>💫 Technical Innovations</strong></summary>

**Custom Occlusion Solution**:
- Material function controlling opacity masks
- Python automation for batch material updates
- Smooth fade transitions without mesh popping
- Performance-optimized compared to failed plugin solutions

**3D Aiming System**:
- Invisible plane collision for accurate cursor placement
- Crosscode-inspired mechanics adapted to 3D space
- Custom trace channels preventing ground interference
- Perspective-independent targeting accuracy

**Event-Driven UI**:
- Delegate-based communication between systems
- No polling-based updates for better performance
- Reactive interface responding to game state changes
- Clean separation of gameplay and presentation logic

</details>

---

## 🔮 Future Vision

While Echo of the Past represents my biggest solo project to date, I see it as a foundation for even more ambitious work. The systems I built here—modular AI, event-driven architecture, and performance-conscious design—are patterns I'll carry into professional game development.

### What I Learned
- **Component-driven design** scales better than monolithic systems
- **Event-based communication** eliminates tight coupling between systems
- **Performance matters early**—architecture decisions compound over time
- **Polish makes the difference** between a demo and a game people want to play

### Potential Expansions
- Further balancing gameplay
- Procedural mission generation for infinite replayability
- Advanced AI director adjusting difficulty dynamically
- Further improving the combat to resemble the complexity of a souls like game

---

## 🤝 Connect & Support

**Ioana Raileanu** - Gameplay Programmer & Designer
- 🌐 **Portfolio**: [juddy2403.github.io](https://juddy2403.github.io)
- 💼 **LinkedIn**: [Connect with me](https://www.linkedin.com/in/ioana-raileanu-147725252/)
- 📧 **Contact**: [Get in touch](mailto:ioanaraileanu24@yahoo.com)
- 🎮 **More Projects**: [GitHub Profile](https://github.com/Juddy2403)

---

## ⭐ Show Your Support

If Echo of the Past impressed you or helped with your own projects, consider:
- ⭐ **Starring this repository**
- 🐛 **Reporting issues** you encounter
- 💡 **Suggesting features** for future development
- 🔄 **Sharing** with other game developers

---

## 📄 License & Credits

This project is available under the **MIT License** - see the [LICENSE](LICENSE) file for details.

**Special Thanks**:
- Epic Games for Unreal Engine 5
- The Crosscode team for combat inspiration
- Digital Arts and Entertainment Belgium for the initial assignment
- The indie game dev community for endless inspiration

---

<div align="center">

**Built with 💖 and lots of ☕ by [Ioana Raileanu](https://juddy2403.github.io)**

*"After all, you are humanity's greatest creation."*

</div>
