// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIVisionCheck.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API UAIVisionCheck : public UBTService
{
	GENERATED_BODY()
	
private:
	// This happens every tick.
	virtual void TickNode (UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackboardComponent *Blackboard;

	// The state of the behavior tree
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector State;

	// The target actor when the AI sees someone.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetActor;
};
