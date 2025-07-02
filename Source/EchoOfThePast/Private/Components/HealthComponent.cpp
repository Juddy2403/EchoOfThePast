// Fill out your copyright notice in the Description page of Project Settings.
#include "Components/HealthComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent(): MaxHealth(100), InvincibilityTime(1.0f), CurrentHealth(100), CanBeHit(true)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::ResetCanBeHit()
{
	CanBeHit = true;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

float UHealthComponent::GetHealthPercentage() const
{
	return CurrentHealth / MaxHealth;
}

bool UHealthComponent::IsDead() const
{
	return CurrentHealth <= 0;
}

void UHealthComponent::Heal(double Amount)
{
	if (Amount <= 0) return;
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0, MaxHealth);
}

void UHealthComponent::SetMaxHP(const double MaxHP)
{
	MaxHealth = MaxHP;
	CurrentHealth = MaxHP;
}

void UHealthComponent::DoDamage_Implementation(float amount, bool isCrit, bool& isDead)
{
	if (CanBeHit)
	{
		UGameplayStatics::SpawnSoundAttached(HitSound, GetOwner()->GetRootComponent());
		CanBeHit = false;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHealthComponent::ResetCanBeHit, InvincibilityTime, false);
		float originalHealth = CurrentHealth;
		CurrentHealth = FMath::Clamp(CurrentHealth - amount, 0.0, MaxHealth);
		if (originalHealth > CurrentHealth && CurrentHealth <= 0) OnDeath.Broadcast();
		OnDamage.Broadcast(amount, isCrit);
	}
	isDead = IsDead();
}

void UHealthComponent::CallOnDeathAfterDelay()
{
	OnDeathAfterDelay.Broadcast();
}

