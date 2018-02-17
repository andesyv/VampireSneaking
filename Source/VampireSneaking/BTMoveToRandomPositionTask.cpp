// Fill out your copyright notice in the Description page of Project Settings.

#include "BTMoveToRandomPositionTask.h"
#include "EnemyAI.h"
#include "AI/Navigation/NavigationSystem.h"

EBTNodeResult::Type UBTMoveToRandomPositionTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEnemyAI *enemyAI = Cast<AEnemyAI>(OwnerComp.GetAIOwner());

	if (enemyAI && enemyAI->GetPossessedPawn()) {
		enemyAI->MoveToLocation(UNavigationSystem::GetRandomPointInNavigableRadius(enemyAI, enemyAI->GetPossessedPawn()->GetActorLocation(), Radius, GetMainNavData()));
		BrainRef = &OwnerComp;
		enemyAI->MoveCompletedDelegate.BindUObject(this, &UBTMoveToRandomPositionTask::finish);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UBTMoveToRandomPositionTask::finish() {
	if (BrainRef) {
		FinishLatentTask(*BrainRef, EBTNodeResult::Succeeded);
		BrainRef = nullptr;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("BTMoveToRandomPositionTask failed!! Something is wrong!"));
	}
}

ANavigationData * UBTMoveToRandomPositionTask::GetMainNavData(FNavigationSystem::ECreateIfEmpty CreateNewIfNoneFound)
{
	UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();
	if (NavSys) {
		return NavSys->GetMainNavData(CreateNewIfNoneFound);
	}
	else {
		return nullptr;
	}
}
