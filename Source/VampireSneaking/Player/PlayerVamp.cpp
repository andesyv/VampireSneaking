// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerVamp.h"
#include "Kismet/GameplayStatics.h"

#include "AI/EnemyAI.h"
#include "Player/DamageType_Explosion.h"
#include "VampireSneakingGameModeBase.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Player/CustomPlayerController.h"

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
	/*
	// Setup for trace-check.
	const FName TraceTag("HitTrace");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams collisionQueryParams{ TraceTag, false };
	*/
	FHitResult hitResult{}; // TODO: Understand how this can show force in the ApplyPointDamage function.
	

	// Check if enemies are inside players attack area
	if (UGameplayStatics::GetGameMode(GetWorld())) {
		AVampireSneakingGameModeBase *gamemode = Cast<AVampireSneakingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gamemode) {
			for (auto item : gamemode->GetEnemyList()) {
				if (item == nullptr) {
					continue;
				}

				/// (Exactly the same checks as in "AIVisionCheck.h". Go see there for explanation.
				FVector playerToEnemy{ item->GetActorLocation() - GetActorLocation() };
				if (playerToEnemy.Size() < AttackRange
					&& AVampireSneakingGameModeBase::GetAngleBetween(playerToEnemy, GetMeshForwardVector()) < AttackAngle
					// && !(GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), item->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel3, collisionQueryParams))
					) {
					UGameplayStatics::ApplyPointDamage(item, AttackDamage, playerToEnemy, hitResult, GetController(), this, DamageType);
				}
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
