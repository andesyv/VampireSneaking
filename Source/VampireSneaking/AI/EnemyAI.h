// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAI.generated.h"

// Forward declarations
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAIPerceptionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskNodeExecutionDelegate, class UBehaviorTreeComponent*, BehaviorTree);

/**
 * Character controller for AI.
 */
UCLASS(Blueprintable)
class VAMPIRESNEAKING_API AEnemyAI : public AAIController
{
	GENERATED_BODY()

protected:
	// Start of the game.
	virtual void BeginPlay() override;

	// Called on possession of controller.
	virtual void Possess(APawn *Pawn) override;

	// Called on unpossession of controller.
	virtual void UnPossess() override;

	// Perception component.
	UAIPerceptionComponent *AIPerceptionComp = nullptr;

public:
	AEnemyAI(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Called on completing current movement request */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	// Delegate for move completion.
	FTaskNodeExecutionDelegate OnMoveCompletedDelegate{};

	const UAIPerceptionComponent* GetPerceptionComp();

	UFUNCTION(Blueprintable)
	void SeeEnemy(APawn *seenPawn);

	UFUNCTION(Blueprintable)
	void DontSeeEnemy();
};
