// Fill out your copyright notice in the Description page of Project Settings.

#include "BTMoveToRandomPositionTask.h"
#include "AI/Navigation/NavigationSystem.h"

EBTNodeResult::Type UBTMoveToRandomPositionTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEnemyAI *enemyAI = Cast<AEnemyAI>(OwnerComp.GetAIOwner());

	if (enemyAI && enemyAI->GetPossessedPawn()) {
		bool recreateMoveRequest{ false };
		do {
			EPathFollowingRequestResult::Type requestResult{ enemyAI->MoveToLocation(UNavigationSystem::GetRandomPointInNavigableRadius(enemyAI, enemyAI->GetPossessedPawn()->GetActorLocation(), Radius, GetMainNavData())) };
			switch (requestResult) {
			case EPathFollowingRequestResult::RequestSuccessful:
				// Succeded, do nothing.
				recreateMoveRequest = false;
				break;
			case EPathFollowingRequestResult::AlreadyAtGoal:
				// Just recreate it and hope it works out better this time.
				recreateMoveRequest = true;
				break;
			case EPathFollowingRequestResult::Failed:
				// Failed! This is an error and should just quit while it's ahead.
				return EBTNodeResult::Failed;
			}
		} while (recreateMoveRequest);

		// UE_LOG(LogTemp, Warning, TEXT("Current move status: %d"), static_cast<int>(enemyAI->GetMoveStatus()));

		if (!(enemyAI->OnMoveCompletedDelegate.IsAlreadyBound(this, &UBTMoveToRandomPositionTask::FinishExecute))) {
			enemyAI->OnMoveCompletedDelegate.AddDynamic(this, &UBTMoveToRandomPositionTask::FinishExecute);
		} else {
			UE_LOG(LogTemp, Error, TEXT("Can't bind delegate!"));
		}

		return EBTNodeResult::InProgress;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Cast failed!"));
	}
	return EBTNodeResult::Failed;
}

bool UBTMoveToRandomPositionTask::UnbindDelegate(UBrainComponent* brainReference) {
	AEnemyAI *enemyAI = Cast<AEnemyAI>(brainReference->GetAIOwner());
	if (enemyAI && enemyAI->OnMoveCompletedDelegate.IsAlreadyBound(this, &UBTMoveToRandomPositionTask::FinishExecute)) {
		enemyAI->OnMoveCompletedDelegate.RemoveDynamic(this, &UBTMoveToRandomPositionTask::FinishExecute);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't unbind delegate!"));
		return false;
	}

	return true;
}

EBTNodeResult::Type UBTMoveToRandomPositionTask::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	UnbindDelegate(&OwnerComp);

	return EBTNodeResult::Aborted;
}

void UBTMoveToRandomPositionTask::FinishExecute(UBehaviorTreeComponent *BehaviorTree) {
	if (BehaviorTree) {
		FinishLatentTask(*BehaviorTree, EBTNodeResult::Succeeded);
		UnbindDelegate(BehaviorTree);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("BTMoveToRandomPositionTask is missing a reference to the brainComponent!"));
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
