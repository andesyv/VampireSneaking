// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Engine/TargetPoint.h"
#include "BTSwitchPatrolPoint.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, DisplayName = "Iterate patrol point")
class VAMPIRESNEAKING_API UBTSwitchPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Called when task is executed.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EBTNodeResult::Type SetBlackboard(UBehaviorTreeComponent& OwnerComp, ATargetPoint * Point);

public:
	// Next patrol point.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector NextPoint;
};
