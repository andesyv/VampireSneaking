// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomPlayerController.h"
#include "VampireSneakingGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API AVampireSneakingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	
	/**
	* Called during RestartPlayer to actually spawn the player's pawn, when using a start spot
	* @param	NewPlayer - Controller for whom this pawn is spawned
	* @param	StartSpot - Actor at which to spawn pawn
	* @return	a pawn of the default pawn class
	*/
	virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;


protected:
	APawn* SpawnBatPawn(const FVector &Position, const FRotator &Rotation);
};
