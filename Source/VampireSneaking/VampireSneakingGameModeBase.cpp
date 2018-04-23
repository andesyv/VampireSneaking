// Fill out your copyright notice in the Description page of Project Settings.

#include "VampireSneakingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Player/CustomPlayerController.h"
#include "Enemy.h"
#include "Player/BatMode.h"
#include "TimerManager.h"

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

void AVampireSneakingGameModeBase::RestartLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName{ *GetWorld()->GetMapName() }, true);
}

//void AVampireSneakingGameModeBase::RestartPlayer(AController * NewPlayer)
//{
//	RestartPlayer(NewPlayer);
//}

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

void AVampireSneakingGameModeBase::PlayerDeath(APlayerController* PlayerCon)
{
	UE_LOG(LogTemp, Warning, TEXT("A player has diedifieded."));
	// If this didn't work, the game can't continue properly. Therefore just crash yoself.
	check(PlayerCon != nullptr);

	if (RespawnTimerHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("What the shit?"));
		return;
	}
	
	FTimerDelegate RespawnTimerDelegate;
	RespawnTimerDelegate.BindUFunction(this, FName{ "RestartPlayer" }, PlayerCon);
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, RespawnTimerDelegate, 2.f, false);
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
