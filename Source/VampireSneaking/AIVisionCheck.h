// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
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
	
	// When the check function can't find give a sensible state.
	NoState	UMETA(DisplayName = "No State"),
};

/**
 * Class that checks to see if the AI can see the player and what action it shall do.
 * Action to do is se in the State enum.
 */
UCLASS(Blueprintable)
class VAMPIRESNEAKING_API UAIVisionCheck : public UBTService
{
	GENERATED_BODY()
	
private:
	// This happens every tick.
	virtual void TickNode (UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;

	float GetAngleBetween(FVector pos1, FVector pos2);

	AIState GetState(UBehaviorTreeComponent & OwnerComp, float DeltaSeconds);
	
public:
	// The state of the behavior tree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector State;

	// The target actor when the AI sees someone.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetActor;

	// The angle from the center that the AI will be able to see, in degrees. Total angle that the AI can see is this * 2.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VisionAngle = 10.f;

	// The length the AI will be able to see.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VisionRadius = 100.f;
};
