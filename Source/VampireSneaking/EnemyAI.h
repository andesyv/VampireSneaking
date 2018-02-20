// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAI.generated.h"

// DECLARE_EVENT_OneParam(AEnemyAI, TaskNodeExecutionDelegate, TSharedPtr<UBehaviorTreeComponent>);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskNodeExecutionDelegate, class UBehaviorTreeComponent*, BehaviorTree);

/**
 * Character controller for AI.
 */
UCLASS()
class VAMPIRESNEAKING_API AEnemyAI : public AAIController
{
	GENERATED_BODY()

private:
	// Called on possession of controller.
	virtual void Possess(APawn *Pawn) override;

	// Called on unpossession of controller.
	virtual void UnPossess() override;

	// Debug pointsArray.
	UPROPERTY(EditDefaultsOnly)
	TArray<FName> PointArr;

	APawn *possessedPawn = nullptr;

public:
	AEnemyAI();

	/** Called on completing current movement request */
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	// Delegate for move completion.
	FTaskNodeExecutionDelegate OnMoveCompletedDelegate{};

	const APawn* GetPossessedPawn() { return possessedPawn; }
};
