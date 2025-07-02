#include "BaseProjectile.h"

#include "Components/HealthComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void ABaseProjectile::OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
	{
		Destroy();
		return;
	}
	ProcessCollision(OtherActor);
}

void ABaseProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		Destroy();
		SpawnHitParticles();
		return;
	}
	ProcessCollision(OtherActor);
}

void ABaseProjectile::ProcessCollision(AActor* OtherActor)
{
	if (OtherActor->ActorHasTag(IgnoreTag)) return;
	//get UHealth component from the hit actor
	if (UHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UHealthComponent>())
	{
		bool bIsDead = false;
		float damageAmount = ComputeDamageAmount();
		HealthComponent->DoDamage_Implementation(damageAmount, damageAmount > DamageAmount, bIsDead);
	}
	SpawnHitParticles();
	Destroy();
}

float ABaseProjectile::ComputeDamageAmount() const
{
	const float CritChance = FMath::FRandRange(0.0f, 100.0f);
	//in case we miss the hit
	if (CritChance <= 10.0f) return 0;
	if (CritChance <= CritRate)
	{
		const float CritMultiplier = FMath::FRandRange(1.5f, 2.0f);
		return FMath::RoundToFloat(DamageAmount * CritMultiplier);
	}
	return DamageAmount;
}

void ABaseProjectile::SpawnHitParticles() const
{
	const FVector Velocity = ProjectileMovement->Velocity;
	const FVector OppositeDirection = -Velocity.GetSafeNormal();
	const FRotator OppositeRotation = OppositeDirection.Rotation() - FRotator(90.0f, 0.0f, 0.0f);
	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		HitParticles, // Your particle effect asset
		GetActorLocation(), // Spawn location
		OppositeRotation   // Rotation in the opposite direction of velocity
	);
	ParticleComponent->SetWorldScale3D(FVector(0.3f));

}

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName("OverlapOnlyPawn");
	Collision->bEditableWhenInherited = true; // Allow editing in derived Blueprints
	RootComponent = Collision;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnComponentBeginOverlap);
	Collision->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnComponentHit);
	Collision->SetGenerateOverlapEvents(true);
	
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	PointLight->SetupAttachment(RootComponent);
	PointLight->bEditableWhenInherited = true; // Allow editing in derived Blueprints

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->bEditableWhenInherited = true; // Allow editing in derived Blueprints

	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	Bullet->SetupAttachment(RootComponent);
	Bullet->SetCollisionProfileName("NoCollision");
	Bullet->bEditableWhenInherited = true; // Allow editing in derived Blueprints


}

