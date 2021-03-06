// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTSwitchPatrolPoint.generated.h"

// Forward declarations
class ATargetPoint;

/**
 * Switches to the next control point.
 */
UCLASS(Blueprintable, DisplayName = "Iterate patrol point")
class VAMPIRESNEAKING_API UBTSwitchPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Called when task is executed.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EBTNodeResult::Type SetBlackboard(UBehaviorTreeComponent& OwnerComp, ATargetPoint * Point, int index);

public:
	// Current patrol point index.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector CurrentPointIndex;

	// Next patrol point.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector NextPoint;
};
