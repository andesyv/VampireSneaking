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

private:
	// Called on possession of controller.
	virtual void Possess(APawn *Pawn) override;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> PointArr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AEnemyAI();


	/*UPROPERTY(VisibleAnywhere)
	UBehaviorTreeComponent *BehaviorTree;*/

	/*UPROPERTY(VisibleAnywhere)
	UBlackboardComponent *Blackboard;*/

};
