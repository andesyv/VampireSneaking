// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VampireSneakingGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDeath);

// Forward declarations
class AEnemy;
class ABatMode;
class ACustomPlayerController;

/**
 * The default Gamemode Base for the Project.
 */
UCLASS()
class VAMPIRESNEAKING_API AVampireSneakingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	friend class ACustomPlayerController;
	
public:
	//////////////////////////////////////////////////////////////////////////
	// Utility functions

	static float GetAngleBetween(FVector pos1, FVector pos2);



public:
	/** Transitions to calls BeginPlay on actors. */
	UFUNCTION(BlueprintCallable, Category = Game)
	void StartPlay() override;

	/**
	* Called during RestartPlayer to actually spawn the player's pawn, when using a start spot
	* @param	NewPlayer - Controller for whom this pawn is spawned
	* @param	StartSpot - Actor at which to spawn pawn
	* @return	a pawn of the default pawn class
	*/
	APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;

	// Restart the level!
	UFUNCTION(BlueprintCallable)
	void RestartLevel();

	// This is literally just a override to have the function in the VampireSneakingGameModeBase class.
	// void RestartPlayer(AController* NewPlayer) override;

	// Get a list of enemies in the scene
	UFUNCTION(BlueprintCallable)
	TArray<AEnemy*>& GetEnemyList();

	/** The bat mode class used when in bat mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<ABatMode> BatFormClass;

protected:
	// For spawning the bat.
	APawn* SpawnBatPawn(UClass *spawnClass, const FVector &Position, const FRotator &Rotation);

	// Resets the enemy blackboard and behaviorTree.
	void ResetEnemyAI(AEnemy* TargetEnemy = nullptr);
	void ResetEnemyAI_Internal(AEnemy* enemy) const;

	FTimerHandle RespawnTimerHandle;

	// Event called when the player dies.
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerDies();

	// Blueprint event that is called when the player dies.
	UPROPERTY(BlueprintAssignable)
	FPlayerDeath OnPlayerDeath;

	// Function called when player dies.
	void PlayerDeath(APlayerController* PlayerCon);

	// List of enemies
	TArray<AEnemy*> EnemyList;

	// Function to remove enemies from list.
	UFUNCTION()
	void RemoveFromEnemyList(AActor *DestroyedEnemy);
};
