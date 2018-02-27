// Fill out your copyright notice in the Description page of Project Settings.

#include "VampireSneakingGameModeBase.h"

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
	}

	return returnValue;
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
