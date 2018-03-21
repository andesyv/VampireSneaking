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
 * The state of the AI, for use in the behaviorTree.
 * (Usable in blueprints and blackboards)
 */
UENUM(BlueprintType)
enum class AIState : uint8 {
	Idle	UMETA(DisplayName = "Idle"),
	Combat	UMETA(DisplayName = "Combat"),
	Searching	UMETA(DisplayName = "Searching"),
	Frozen	UMETA(DisplayName = "Frozen"),
	
	// When the check function can't find give a sensible state.
	NoState	UMETA(DisplayName = "No State"),
};

/**
 * Character controller for AI.
 */
UCLASS(Blueprintable)
class VAMPIRESNEAKING_API AEnemyAI : public AAIController
{
	GENERATED_BODY()

	friend class APlayerVamp;

protected:
	// Start of the game.
	virtual void BeginPlay() override;

	// Called on possession of controller.
	virtual void Possess(APawn *Pawn) override;

	// Called on unpossession of controller.
	virtual void UnPossess() override;

	// Timerhandle for the searching.
	FTimerHandle SearchingTimerHandle;

	// For setting the AI to the Idle state
	UFUNCTION()
	void SetAIIdleState();

	// Perception component.
	UAIPerceptionComponent *AIPerceptionComp = nullptr;

	// Called whenever AI Perception updates it's state.
	UFUNCTION(BlueprintCallable)
	void UpdateState(TArray<AActor*> UpdatedActors);

	/**
	 * Toggles the blackboard state enum between Frozen and Idle.
	 * Returns true if state was changed to Frozen.
	 */
	bool ToggleSucking();

	AIState lastState;

public:
	AEnemyAI(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Called on completing current movement request */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	// Delegate for move completion.
	FTaskNodeExecutionDelegate OnMoveCompletedDelegate{};

	UAIPerceptionComponent* const GetPerceptionComp();

	// How long the AI will search for before going back into patrol-mode
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SearchTime = 3.f;
};
