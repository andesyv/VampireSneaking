// Fill out your copyright notice in the Description page of Project Settings.

#include "VampireSneakingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Player/CustomPlayerController.h"
#include "Enemy.h"
#include "Player/BatMode.h"
#include "TimerManager.h"
#include "AI/EnemyAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "HealthComponent.h"

void AVampireSneakingGameModeBase::StartPlay()
{
	if (EnemyList.Num() > 0) {
		EnemyList.Empty();
	}
	// Add all enemies in the scene to the enemy list.
	for (TActorIterator<AEnemy> Itr(GetWorld()); Itr; ++Itr) {
		EnemyList.Add(*Itr);
		Itr->OnDestroyed.AddDynamic(this, &AVampireSneakingGameModeBase::RemoveFromEnemyList);
	}

	Super::StartPlay();
}

APawn* AVampireSneakingGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) {
	APawn *returnValue = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	// Check if the Controllable Pawns already got stuff in it. (Hopefully doesn't. :s)
	ACustomPlayerController *playerController = Cast<ACustomPlayerController>(NewPlayer);
	if (playerController && playerController->ControllablePawns.Num() < 1) {

		// Add player-pawn.
		playerController->ControllablePawns.Add(returnValue);
		
		// Add bat-pawn.
		APawn *bat = SpawnBatPawn(BatFormClass, FVector{ 0.f, 0.f, -1000.f }, FRotator::ZeroRotator);
		if (bat) {
			playerController->ControllablePawns.Add(bat);
		}

		// Make stats
	}

	return returnValue;
}

void AVampireSneakingGameModeBase::RestartPlayer(AController * NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	if (NewPlayer)
	{
		auto *controller = Cast<ACustomPlayerController>(NewPlayer);
		if (controller && controller->HealthComponent)
		{
			// controller->HealthComponent->Reset();
		}
	}
}

void AVampireSneakingGameModeBase::RestartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName{ *GetWorld()->GetMapName() }, true);
}

TArray<AEnemy*>& AVampireSneakingGameModeBase::GetEnemyList()
{
	return EnemyList;
}

APawn * AVampireSneakingGameModeBase::SpawnBatPawn(UClass *spawnClass, const FVector & Position, const FRotator & Rotation)
{
	APawn *returnPawn{ nullptr };
	UWorld *world = GetWorld();

	FTransform transform{ Rotation, Position, FVector{1.f, 1.f, 1.f} };
	// FActorSpawnParameters spawnParams;

	if (world && spawnClass) {
		returnPawn = world->SpawnActor<APawn>(spawnClass, transform);

		if (!returnPawn) {
			UE_LOG(LogTemp, Error, TEXT("Can't spawn bat!"));
		}
	}

	return returnPawn;
}

void AVampireSneakingGameModeBase::ResetEnemyAI(AEnemy* TargetEnemy)
{
	if (TargetEnemy)
	{
		ResetEnemyAI_Internal(TargetEnemy);
	}
	else
	{
		for (const auto &enemy : GetEnemyList())
		{
			ResetEnemyAI_Internal(enemy);
		}
	}
}

void AVampireSneakingGameModeBase::ResetEnemyAI_Internal(AEnemy* enemy) const
{
	if (enemy && enemy->GetController())
	{
		AEnemyAI *enemyAI = Cast<AEnemyAI>(enemy->GetController());
		if (enemyAI && enemyAI->BrainComponent)
		{
			UBehaviorTreeComponent * behaviorTree = Cast<UBehaviorTreeComponent>(enemyAI->BrainComponent);
			if (behaviorTree)
			{
				if (behaviorTree->GetBlackboardComponent() && enemy->BehaviorTree)
				{
					// Reinitialize blackboard and restart behavior.
					behaviorTree->GetBlackboardComponent()->InitializeBlackboard(*enemy->BehaviorTree->BlackboardAsset);
					behaviorTree->RestartTree();

					const auto numberOfKeys = behaviorTree->GetBlackboardComponent()->GetNumKeys();
					for (uint8 i{0}; i < numberOfKeys; ++i)
					{
						// Don't reset patrol values
						if (behaviorTree->GetBlackboardComponent()->GetKeyName(i) == FName{"NextPoint"} || behaviorTree->GetBlackboardComponent()->GetKeyName(i) == FName{ "CurrentPointIndex" })
						{
							continue;
						}
						behaviorTree->GetBlackboardComponent()->ClearValue(i);
					}
				}
			}
		}
	}
}

void AVampireSneakingGameModeBase::PlayerDeath(APlayerController* PlayerCon)
{
	UE_LOG(LogTemp, Warning, TEXT("A player has diedifieded."));
	// If this didn't work, the game can't continue properly. Therefore just crash yoself.
	check(PlayerCon != nullptr);

	check(GetWorld() != nullptr);

	if (RespawnTimerHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle))
	{
		UE_LOG(LogTemp, Error, TEXT("Another player is currently respawning?"));
		return;
	}

	ResetEnemyAI();
	
	FTimerDelegate RespawnTimerDelegate;
	RespawnTimerDelegate.BindUFunction(this, FName{ "RestartPlayer" }, PlayerCon);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, RespawnTimerDelegate, 2.f, false);
	// RestartPlayer(PlayerCon);

	OnPlayerDeath.Broadcast();
}

void AVampireSneakingGameModeBase::RemoveFromEnemyList(AActor * DestroyedEnemy)
{
	if (DestroyedEnemy) {
		AEnemy *enemy = Cast<AEnemy>(DestroyedEnemy);
		if (enemy) {
			EnemyList.Remove(enemy);
		}
	}
}

float AVampireSneakingGameModeBase::GetAngleBetween(FVector pos1, FVector pos2)
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(pos1, pos2) / (pos1.Size() * pos2.Size())));
}
