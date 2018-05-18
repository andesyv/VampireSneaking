// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->UpdatedComponent = CollisionComponent;
	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::BloodHit);
}

void AProjectile::BloodHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == this)
	{
		return;
	}

	// Make a sound
	if (HitSounds.Num() > 0)
	{
		const auto RandomIndex = FMath::RandRange(0, HitSounds.Num() - 1);
		UGameplayStatics::PlaySoundAtLocation(this, HitSounds[RandomIndex], GetActorLocation(), FRotator::ZeroRotator);
	}
	MakeNoise(1, Instigator, GetActorLocation(), 2000.f);

	// Play a particleeffect
	if (HitParticleEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticleEffect, GetActorLocation());
	}

	MovementComponent->Velocity = FVector::ZeroVector;
	CollisionComponent->SetSimulatePhysics(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestroyTimerHandle{};
	FTimerDelegate TimerDelegate;

	/// Check it; a lambda!
	TimerDelegate.BindLambda([&]() { Destroy(); });
	check(GetWorld() != nullptr);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, TimerDelegate, 1.5f, false);
}
