// Fill out your copyright notice in the Description page of Project Settings.

#include "VampireSneakingGameModeBase.h"

APawn* AVampireSneakingGameModeBase::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) {
	APawn *returnValue = Super::SpawnDefaultPawnFor_Implementation(NewPlayer, StartSpot);

	ACustomPlayerController *playerController = Cast<ACustomPlayerController>(NewPlayer);
	if (playerController && playerController->ControllablePawns.Num() < 1) {
		// Add player-pawn.
		playerController->ControllablePawns.Add(returnValue);
		
		// Add bat-pawn.
		playerController->ControllablePawns.Add(SpawnBatPawn(FVector{ 0.f, 0.f, 0.f }, FRotator::ZeroRotator));
	}

	return returnValue;
}

APawn * AVampireSneakingGameModeBase::SpawnBatPawn(const FVector & Position, const FRotator & Rotation)
{
	// TODO

	return nullptr;
}
