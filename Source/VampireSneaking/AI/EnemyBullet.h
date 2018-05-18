// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

// Forward declarations
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class VAMPIRESNEAKING_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBullet();

	// Damage bullet does.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 30.f;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProjectileMovementComponent *MovementComponent;

	// Collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USphereComponent *CollisionComponent;
	
	// Called when actor overlaps something
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
};
