// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AI/EnemyAI.h"
#include "AIVisionCheck.generated.h"

// Forward declarations
class UBlackboardComponent;

/**
 * Class that checks to see if the AI can see the player and what action it shall do.
 * Action to do is se in the State enum.
 */
UCLASS(deprecated, Blueprintable, DisplayName = "Vision Check")
class VAMPIRESNEAKING_API UDEPRECATED_AIVisionCheck : public UBTService
{
	GENERATED_BODY()
	
protected:
	// This happens every tick.
	virtual void TickNode (UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

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
