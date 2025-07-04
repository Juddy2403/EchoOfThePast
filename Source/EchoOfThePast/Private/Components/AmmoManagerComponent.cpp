// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AmmoManagerComponent.h"

// Sets default values for this component's properties
UAmmoManagerComponent::UAmmoManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UAmmoManagerComponent::ConsumeAmmo()
{
	if (CurrentAmmo > 0)
	{
		CurrentAmmo--;
		OnAmmoChanged.Broadcast(CurrentAmmo);
	}
}

void UAmmoManagerComponent::AddAmmo(int32 Amount)
{
	CurrentAmmo += Amount;
	OnAmmoChanged.Broadcast(CurrentAmmo);
}

int32 UAmmoManagerComponent::GetCurrentAmmo() const
{
	return CurrentAmmo;
}

bool UAmmoManagerComponent::HasAmmo() const
{
	return CurrentAmmo > 0;
}

