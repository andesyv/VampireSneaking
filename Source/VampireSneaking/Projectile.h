// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

// Forward declarations
class UProjectileMovementComponent;
class USphereComponent;

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
	// Called every frame
	void Tick(float DeltaTime) override;

	APawn * Instigator{ nullptr };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BloodHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Sounds the blood projectile can make when hitting something
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USoundBase*> HitSounds;

	
};
