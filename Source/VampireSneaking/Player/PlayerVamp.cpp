// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerVamp.h"
#include "Kismet/GameplayStatics.h"
#include "AI/EnemyAI.h"
#include "Player/DamageType_Explosion.h"
#include "VampireSneakingGameModeBase.h"
#include "Enemy.h"
#include "Player/CustomPlayerController.h"
#include "HealthComponent.h"
#include "Projectile.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlayerVamp::APlayerVamp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(0);
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
	PlayerInputComponent->BindAction("BloodAttack", IE_Pressed, this, &APlayerVamp::BloodAttack);
}

bool APlayerVamp::ToggleBloodSucking() {
	if (suckedEnemy->GetController()) {
		AEnemyAI *enemyAI = Cast<AEnemyAI>(suckedEnemy->GetController());
		if (enemyAI) {
			return enemyAI->ToggleSucking();
		}
	}
	return false;
}

void APlayerVamp::SuckBlood(float amount, float DeltaTime)
{
	// Toggling ability.
	if (GetController()) {
		ACustomPlayerController *playerCon = Cast<ACustomPlayerController>(GetController());
		if (playerCon) {
			if (playerCon->GetBloodSuckButton() && EnemyInFront()) {
				SuckingBlood = !SuckingBlood ? ToggleBloodSucking() : SuckingBlood;
			} else {
				SuckingBlood = SuckingBlood ? ToggleBloodSucking() : SuckingBlood;
			}

			// No need to shorten this logic down even further, just makes it confusing to debug.
			// SuckingBlood = (playerCon->GetBloodSuckButton() && EnemyInFront()) ? (!SuckingBlood ? ToggleBloodSucking() : SuckingBlood) : (SuckingBlood ? ToggleBloodSucking() : SuckingBlood);

			// Do the sucking.
			if (SuckingBlood && suckedEnemy && playerCon->HealthComponent && suckedEnemy->GetController()) {
				playerCon->HealthComponent->AddBlood(amount * DeltaTime);
				AEnemyAI *enemyAI = Cast<AEnemyAI>(suckedEnemy->GetController());
				if (enemyAI && enemyAI->HealthComponent) {
					enemyAI->HealthComponent->AddBlood(-amount * DeltaTime);
				} else {
					UE_LOG(LogTemp, Warning, TEXT("Can't fetch healthcomponent on enemyAI."));
				}				
			} else if (SuckingBlood) {
				UE_LOG(LogTemp, Warning, TEXT("Can't fetch suckedEnemy's controller."));
			}
		}
	}
}

void APlayerVamp::Attack_Implementation()
{
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

FGenericTeamId APlayerVamp::GetGenericTeamId() const
{
    return TeamId;
}

void APlayerVamp::BloodAttack() {

	
		UWorld *world = GetWorld();
		if(world){
			//Spawning the bullet
		world->SpawnActor<AProjectile>(ProjectileBlueprint, GetActorLocation(), GetActorForwardVector().Rotation());
		UE_LOG(LogTemp, Warning, TEXT("finished"));

	}
}