// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/PlayerVamp.h"
#include "Player/CustomPlayerController.h"
#include "HealthComponent.h"


// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision"));
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->UpdatedComponent = CollisionComponent;
	CollisionComponent->OnComponentHit.AddDynamic(this, &AEnemyBullet::OnHit);
}

void AEnemyBullet::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherActor->IsA(APlayerVamp::StaticClass()))
	{
		auto *playerVamp = Cast<APlayerVamp>(OtherActor);
		if (playerVamp && playerVamp->GetController())
		{
			auto *playerController = Cast<ACustomPlayerController>(playerVamp->GetController());
			if (playerController && playerController->HealthComponent)
			{
				playerController->HealthComponent->TakeDamage(Damage);
			}
		}
	}
	Destroy();
}
