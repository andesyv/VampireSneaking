// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/PrimitiveComponent.h"
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
	UE_LOG(LogTemp, Warning, TEXT("There's another list, but no one really want to think about this"))
		UPrimitiveComponent *component = FindComponentByClass<UStaticMeshComponent>();
	if (component) {
		component->SetPhysicsAngularVelocity(FVector{ FMath::RandRange(0.1f, 1.f), FMath::RandRange(0.1f, 1.f), FMath::RandRange(0.1f, 1.f) } *1000.f);
		UE_LOG(LogTemp, Error, TEXT("Bullets either way, suicidal crazed lion."));
		//component->OnComponentHit.AddDynamic(this, &AProjectile::BloodHit);
	}
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::BloodHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit, float amount, float DeltaTime) {
	/* if (OtherActor != this && OtherActor->IsA(AEnemy::StaticClass())) {
		AEnemy *enemy{ Cast<AEnemy>(OtherActor) };
		//Same as when player sucks blood

		AEnemyAI *enemyAI = Cast<AEnemyAI>(enemy);
		if (enemyAI && enemyAI->HealthComponent) {
			enemyAI->HealthComponent->AddBlood(-amount * DeltaTime);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Can't fetch healthcomponent on enemyAI."));
		}
		
	} */
}
