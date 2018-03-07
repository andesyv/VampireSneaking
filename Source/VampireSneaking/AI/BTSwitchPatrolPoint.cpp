// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTSwitchPatrolPoint.h"
#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"
#include "EngineUtils.h"


EBTNodeResult::Type UBTSwitchPatrolPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn() && OwnerComp.GetBlackboardComponent()) {
		AEnemy *enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

		int currentPointIndexInstance{ OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Int>(CurrentPointIndex.SelectedKeyName) };
		if (enemy && currentPointIndexInstance >= 0) {
			int ArraySize{ enemy->PatrolPoints.Num() };
			int emptySpots{ 0 };
			for (auto item : enemy->PatrolPoints) {
				if (!item) {
					emptySpots++;
				}
			}
			if (ArraySize > emptySpots) {

				do {
					// Increment. Will loop around to the first index again, even if you remove an index.
					currentPointIndexInstance = (ArraySize < currentPointIndexInstance) ? 0 : (currentPointIndexInstance + 1) % ArraySize;

					// Check if current index is empty
				} while (!enemy->PatrolPoints[currentPointIndexInstance]);

				return SetBlackboard(OwnerComp, enemy->PatrolPoints[currentPointIndexInstance], currentPointIndexInstance);
			}
			else {
				TArray<ATargetPoint*> PatrolPoints{};

				// Add all ATargetPoint's to the Patrol Points array.
				for (TActorIterator<ATargetPoint> Itr(GetWorld()); Itr; ++Itr) {
					if (*Itr) {
						PatrolPoints.Add(*Itr);
					}
				}

				if (PatrolPoints.Num() > 0) {
					PatrolPoints.Sort();

					// Increment. Will loop around to the first index again, even if you remove an index.
					currentPointIndexInstance = (PatrolPoints.Num() < currentPointIndexInstance) ? 0 : (currentPointIndexInstance + 1) % PatrolPoints.Num();

					return SetBlackboard(OwnerComp, PatrolPoints[currentPointIndexInstance], currentPointIndexInstance);
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTSwitchPatrolPoint::SetBlackboard(UBehaviorTreeComponent& OwnerComp, ATargetPoint * Point, int index)
{
	UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();
	if (blackboard && Point && !(blackboard->SetValue<UBlackboardKeyType_Vector>(NextPoint.SelectedKeyName, Point->GetActorLocation()))) {
		UE_LOG(LogTemp, Error, TEXT("Failed to set next patrol point in blackboard!"));
		return EBTNodeResult::Failed;
	}

	if (blackboard && !(blackboard->SetValue<UBlackboardKeyType_Int>(CurrentPointIndex.SelectedKeyName, index))) {
		UE_LOG(LogTemp, Error, TEXT("Failed to set current point index in blackboard!"));
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}
