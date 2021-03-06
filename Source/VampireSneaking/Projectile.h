// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// Forward declarations
class UProjectileMovementComponent;
class USphereComponent;
class UPawnNoiseEmitterComponent;

UCLASS()
class VAMPIRESNEAKING_API AProjectile : public AActor
{

	GENERATED_BODY()

	friend class APlayerVamp;
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent *MovementComponent;

	// Collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USphereComponent *CollisionComponent;

public:	

	APawn * Instigator{ nullptr };

	/**
	 * The range of the sound the projectile makes when it hits something.
	 * Should be atleast as big as the AI's hearing range.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SoundRange = 2000.f;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BloodHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Sounds the blood projectile can make when hitting something
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> HitSounds;

	// Particle to spawn when the projectile hits something.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem *HitParticleEffect;
	
};
