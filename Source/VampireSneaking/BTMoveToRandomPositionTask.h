// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAI.h"
#include "BTMoveToRandomPositionTask.generated.h"

/**
 * Moves to a random point within radius.
 */
UCLASS()
class VAMPIRESNEAKING_API UBTMoveToRandomPositionTask : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	// Called when task is executed.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	ANavigationData* GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound = FNavigationSystem::Create);

	bool UnbindDelegate(UBrainComponent* brainReference);

	UBehaviorTreeComponent *BrainRef{ nullptr };

protected:
	// Called when task is aborted.
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void FinishExecute(FAIRequestID RequestID, EPathFollowingResult::Type Result);

public:
	// The radius to create random points in, centered on the enemy.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 100.f;
};
