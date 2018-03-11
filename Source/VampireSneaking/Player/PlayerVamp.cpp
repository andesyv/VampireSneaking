// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerVamp.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "AI/EnemyAI.h"
#include "Player/DamageType_Explosion.h"

// Sets default values
APlayerVamp::APlayerVamp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerVamp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerVamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SuckBlood(SuckSpeed, DeltaTime);
}

// Called to bind functionality to input
void APlayerVamp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerVamp::Attack);
}

void APlayerVamp::SuckBlood(float amount, float DeltaTime)
{
	// TODO: Make more efficient?
	if (GetController()) {
		ACustomPlayerController *playerCon = Cast<ACustomPlayerController>(GetController());
		if (playerCon) {
			if (playerCon->GetBloodSuckButton() && EnemyInFront()) {
				if (EnemyLocked != true) {
					EnemyLocked = true;
					suckedEnemy->beingSucked = EnemyLocked;
				}
				playerCon->AddBlood(amount * DeltaTime);
			}
			else if (EnemyLocked != false) {
				EnemyLocked = false;
				suckedEnemy->beingSucked = EnemyLocked;
				suckedEnemy = nullptr;
			}
		}
	}
}

void APlayerVamp::Attack()
{
	const FName TraceTag("HitTrace");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams collisionQueryParams{ TraceTag, false , this };

	FHitResult hitResult{};
	if (GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + GetMeshForwardVector()*1000.f, ECollisionChannel::ECC_WorldDynamic, collisionQueryParams))
	{
		TArray<AActor*> IgnoredActors{};
		IgnoredActors.Add(this);
		float baseDmg{ 100.f };
		UE_LOG(LogTemp, Warning, TEXT("Damage applied: %f"), baseDmg);
		UGameplayStatics::ApplyRadialDamage(GetWorld(), baseDmg, hitResult.ImpactPoint, 10000.f, DamageType, IgnoredActors, this, GetController(), false, ECC_Visibility);

		AEnemy* enemy = Cast<AEnemy>(hitResult.Actor.Get());
		if (enemy && enemy->GetMovementComponent())
		{
			UCharacterMovementComponent *movement = Cast<UCharacterMovementComponent>(enemy->GetMovementComponent());
			if (movement) {
				/*FVector toEnemy{ enemy->GetActorLocation() - GetActorLocation() };
				toEnemy.GetSafeNormal();*/
				// movement->AddImpulse(FVector{ enemy->GetActorLocation() - GetActorLocation() }.GetSafeNormal() * HitForce);
				
			}
		}
	}
}

bool APlayerVamp::EnemyInFront()
{
	const FName TraceTag("SuckTrace");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams collisionQueryParams{ TraceTag, false , this };

	FHitResult hitResult{};
	if (GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + GetMeshForwardVector()*100.f, ECollisionChannel::ECC_WorldDynamic, collisionQueryParams))
	{
		AEnemy* enemy = Cast<AEnemy>(hitResult.Actor.Get());
		if (enemy)
		{
			suckedEnemy = enemy;
			return true;
		}
	}
	return false;
}
