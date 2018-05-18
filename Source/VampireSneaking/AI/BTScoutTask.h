// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTScoutTask.generated.h"

/**
 * This task sways in the specified angle at the specified frequency.
 */
UCLASS(Blueprintable, meta = ( DisplayName = "Swaying task"))
class VAMPIRESNEAKING_API UBTScoutTask : public UBTTaskNode
{
	GENERATED_BODY()
	
	/** starts this task, should return Succeeded, Failed or InProgress
	*  (use FinishLatentTask() when returning InProgress)
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	/** ticks this task
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	
	FRotator StartRotation{};
	float timer{ 0.f };

public:
	// Sway amount
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount = 30.f;

	// Sway frequency
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency = 1.f;
	
};
