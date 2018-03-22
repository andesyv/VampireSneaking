// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTSetBool.generated.h"

/**
 * Task that sets a bool on the blackboard.
 */
UCLASS(Blueprintable, DisplayName = "Set blackboard bool")
class VAMPIRESNEAKING_API UBTSetBool : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Called when task is executed.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// Value to set bool
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Value = true;
	
	// Bool key to change
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector BoolKey;	
};
