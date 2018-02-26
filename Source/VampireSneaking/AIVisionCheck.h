// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AIVisionCheck.generated.h"

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
 * Class that checks to see if the AI can see the player and what action it shall do.
 * Action to do is se in the State enum.
 */
UCLASS(Blueprintable, DisplayName = "Vision Check")
class VAMPIRESNEAKING_API UAIVisionCheck : public UBTService
{
	GENERATED_BODY()
	
private:
	// This happens every tick.
	virtual void TickNode (UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

	float GetAngleBetween(FVector pos1, FVector pos2);

	AIState GetState(UBehaviorTreeComponent & OwnerComp, float DeltaSeconds, AIState lastState);
	
public:
	// The state of the behavior tree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector State;

	// Position where the AI last saw the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector LastSeenPosition;

	// The player, when the AI sees it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetActor;

	// The amount of time the AI will search for before giving up and going back to it's patrol.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SearchTime = 3.f;
};
