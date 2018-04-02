// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"
#include "Enemy.h"
#include "AI/EnemyAI.h"


// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Starting beginplay"))
		UPrimitiveComponent *component = FindComponentByClass<UStaticMeshComponent>();
	if (component) {
		component->AddImpulse(GetActorForwardVector() * 100000.f);
		component->SetPhysicsAngularVelocity(FVector{ FMath::RandRange(0.1f, 1.f), FMath::RandRange(0.1f, 1.f), FMath::RandRange(0.1f, 1.f) } *1000.f);
		UE_LOG(LogTemp, Error, TEXT("Doest it bother anyone else that someone else has your name?"));
		component->OnComponentHit.AddDynamic(this, &AProjectile::BloodHit);
	}
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::BloodHit( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor != this && OtherActor->IsA(AEnemy::StaticClass())) {
		AEnemy *enemy{ Cast<AEnemy>(OtherActor) };
		//Same as when player sucks blood

		AEnemyAI *enemyAI = Cast<AEnemyAI>(enemy);
		if (enemyAI && enemyAI->HealthComponent) {
			// lose enemy health here
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("The enemy did not lose health"));
		}
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Function successfully running through"));
}