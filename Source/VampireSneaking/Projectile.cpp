// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	

	//TArray<UStaticMeshComponent*> meshArray;
	//// UPrimitiveComponent* component = FindComponentByClass<UStaticMeshComponent>();
	//GetComponents<UStaticMeshComponent>(meshArray);
	//for (auto item : meshArray)
	//{
	//	if (item)
	//	{
	//		item->AddImpulse(GetActorForwardVector() * 100000.f);
	//		item->SetPhysicsAngularVelocityInDegrees(FVector{
	//			FMath::RandRange(0.1f, 1.f), FMath::RandRange(0.1f, 1.f), FMath::RandRange(0.1f, 1.f) * 1000.f
	//		});
	//		item->OnComponentHit.AddDynamic(this, &AProjectile::BloodHit);
	//	}
	//}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::BloodHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
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
	MakeNoise(1, Instigator, GetActorLocation(), 400);

	Destroy();
}
