// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSwitchPatrolPoint.h"
#include "AIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "EngineUtils.h"


EBTNodeResult::Type UBTSwitchPatrolPoint::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	static int32 currentPointIndex{ 0 };

	if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn()) {
		AEnemy *enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
		if (enemy) {
			int32 ArraySize{ enemy->PatrolPoints.Num() };
			bool bEmptySpots{ false };
			for (auto item : enemy->PatrolPoints) {
				if (!item) {
					bEmptySpots = true;
				}
			}
			if (ArraySize > 0 && !bEmptySpots) {

				// Increment. Will loop around to the first index again, even if you remove an index.
				currentPointIndex = (ArraySize < currentPointIndex) ? 0 : (currentPointIndex + 1) % ArraySize;

				return SetBlackboard(OwnerComp, enemy->PatrolPoints[currentPointIndex]);
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
					currentPointIndex = (PatrolPoints.Num() < currentPointIndex) ? 0 : (currentPointIndex + 1) % PatrolPoints.Num();

					return SetBlackboard(OwnerComp, PatrolPoints[currentPointIndex]);
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTSwitchPatrolPoint::SetBlackboard(UBehaviorTreeComponent& OwnerComp, ATargetPoint * Point)
{
	UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();
	if (blackboard && Point && !(blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID(NextPoint.SelectedKeyName), Point->GetActorLocation()))) {
		UE_LOG(LogTemp, Error, TEXT("Failed to set next patrol point in blackboard!"));
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Succeeded;
}
