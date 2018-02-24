// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnemyAI.h"
#include "Engine/TargetPoint.h"
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

	FVector GetCenterPoint(const UBehaviorTreeComponent& OwnerComp) const;

protected:
	// Called when task is aborted.
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void FinishExecute(UBehaviorTreeComponent *BehaviorTree);

public:
	// The radius to create random points in, centered on the enemy.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 100.f;

	// TODO: Remake this task node so that is uses the MoveTo -task node instead of handling moving on it's own.
	// And make this task node able to center around a target point.

	// Move around self instead of given point?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MoveAroundSelf = true;

	// Point to randomly move around.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Center = FBlackboardKeySelector{};
};
