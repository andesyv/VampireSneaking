// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Player/PlayerVamp.h"
#include "Player/CustomPlayerController.h"
#include "HealthComponent.h"


// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->UpdatedComponent = CollisionComponent;
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
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
