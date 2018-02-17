// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTMoveToRandomPositionTask.generated.h"

/**
 * Moves to a random point within radius.
 */
UCLASS()
class VAMPIRESNEAKING_API UBTMoveToRandomPositionTask : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	ANavigationData* GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound = FNavigationSystem::Create);

	/** helper function: finish latent executing */
	// void FinishLatentTask(UBehaviorTreeComponent& OwnerComp, EBTNodeResult::Type TaskResult) const;

	UBehaviorTreeComponent *BrainRef{ nullptr };

	void finish();

public:

	// The radius to create random points in, centered on the enemy.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 100.f;
};
