#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UPointLightComponent;

UCLASS()
class ECHOOFTHEPAST_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComponent,
						FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
	UFUNCTION()
	void ProcessCollision(AActor* OtherActor);

	UFUNCTION()
	float ComputeDamageAmount() const;

	UFUNCTION()
	void SpawnHitParticles() const;

public:
	ABaseProjectile();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	TObjectPtr<UPointLightComponent> PointLight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> Bullet;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Default")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UParticleSystem* HitParticles;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	float DamageAmount;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FName IgnoreTag;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	float CritRate;
};
