// Fill out your copyright notice in the Description page of Project Settings.

#include "BTMoveToRandomPositionTask.h"
// #include "EnemyAI.h"
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

		UE_LOG(LogTemp, Warning, TEXT("Current move status: %d"), static_cast<int>(enemyAI->GetMoveStatus()));
		BrainRef = &OwnerComp;
		// enemyAI->MoveCompletedDelegate.BindUObject(this, &UBTMoveToRandomPositionTask::FinishExecute);
		if (!(enemyAI->ReceiveMoveCompleted.IsAlreadyBound(this, &UBTMoveToRandomPositionTask::FinishExecute))) {
			enemyAI->ReceiveMoveCompleted.AddDynamic(this, &UBTMoveToRandomPositionTask::FinishExecute);
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

void UBTMoveToRandomPositionTask::FinishExecute(FAIRequestID RequestID, EPathFollowingResult::Type Result) {
	if (BrainRef) {
		FinishLatentTask(*BrainRef, EBTNodeResult::Succeeded);
		AEnemyAI *enemyAI = Cast<AEnemyAI>(BrainRef->GetAIOwner());
		if (enemyAI && enemyAI->ReceiveMoveCompleted.IsAlreadyBound(this, &UBTMoveToRandomPositionTask::FinishExecute)) {
			// enemyAI->MoveCompletedDelegate.Unbind();
			enemyAI->ReceiveMoveCompleted.RemoveDynamic(this, &UBTMoveToRandomPositionTask::FinishExecute);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Can't unbind delegate!"));
		}

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
