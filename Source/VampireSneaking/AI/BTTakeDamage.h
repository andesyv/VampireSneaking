// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTakeDamage.generated.h"

/**
 * Task that deals damage to the enemy and pushes the enemy away.
 */
UCLASS(Blueprintable, DisplayName = "Take damage")
class VAMPIRESNEAKING_API UBTTakeDamage : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Called when task is executed.
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	// State enum blackboard key
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector State;

	// Reset attack cooldown blackboard key
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector ResetAttackCooldown;
};
