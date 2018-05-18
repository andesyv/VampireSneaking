// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTConstantRotation.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API UBTConstantRotation : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Called when task is executed.
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** ticks this task
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
