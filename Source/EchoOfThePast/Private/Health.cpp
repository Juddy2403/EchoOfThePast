// Fill out your copyright notice in the Description page of Project Settings.
#include "Health.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealth::UHealth(): MaxHealth(100), InvincibilityTime(1.0f), CurrentHealth(100), CanBeHit(true)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealth::ResetCanBeHit()
{
	CanBeHit = true;
}

// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float UHealth::GetHealthPercentage() const
{
	return CurrentHealth / MaxHealth;
}

bool UHealth::IsDead() const
{
	return CurrentHealth <= 0;
}

void UHealth::Heal(double Amount)
{
	if (Amount <= 0) return;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0, MaxHealth);
}

void UHealth::SetMaxHP(const double MaxHP)
{
	MaxHealth = MaxHP;
	CurrentHealth = MaxHP;
}

void UHealth::DoDamage_Implementation(float amount, bool isCrit, bool& isDead)
{
	if (CanBeHit)
	{
		UGameplayStatics::SpawnSoundAttached(HitSound, GetOwner()->GetRootComponent());
		CanBeHit = false;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHealth::ResetCanBeHit, InvincibilityTime, false);
		float originalHealth = CurrentHealth;
		CurrentHealth = FMath::Clamp(CurrentHealth - amount, 0.0, MaxHealth);
		if (originalHealth > CurrentHealth && CurrentHealth <= 0) OnDeath.Broadcast();
		OnDamage.Broadcast(amount, isCrit);
	}
	isDead = IsDead();
}

