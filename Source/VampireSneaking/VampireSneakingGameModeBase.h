// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CustomPlayerController.h"
#include "BatMode.h"
#include "VampireSneakingGameModeBase.generated.h"

/**
 * The default Gamemode Base for the Project.
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

	/** The bat mode class used when in bat mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ABatMode> BatFormClass;

protected:
	// For spawning the bat.
	APawn* SpawnBatPawn(UClass *spawnClass, const FVector &Position, const FRotator &Rotation);
};
