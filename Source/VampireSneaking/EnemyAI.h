// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAI.generated.h"

/**
 * Character controller for AI.
 */
UCLASS()
class VAMPIRESNEAKING_API AEnemyAI : public AAIController
{
	GENERATED_BODY()
	DECLARE_DELEGATE(OnMoveCompletedDelegate);

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

	OnMoveCompletedDelegate MoveCompletedDelegate{};

	const APawn* GetPossessedPawn() { return possessedPawn; }
};
