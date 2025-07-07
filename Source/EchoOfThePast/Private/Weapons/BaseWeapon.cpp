// Fill out your copyright notice in the Description page of Project Settings.
#include "Weapons/BaseWeapon.h"

#include "DataTypes/DamageTypeEnum.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and attach the Skeletal Mesh Component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	WeaponMesh->SetupAttachment(RootComponent);

	// Allow customization in Blueprints or the editor
	WeaponMesh->bEditableWhenInherited = true; // Allow editing in derived Blueprints
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}
