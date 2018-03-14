// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTMoveToRandomPositionTask.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/EnemyAI.h"
#include "Engine/TargetPoint.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTMoveToRandomPositionTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEnemyAI *enemyAI = Cast<AEnemyAI>(OwnerComp.GetAIOwner());

	if (enemyAI && enemyAI->GetPossessedPawn()) {
		bool recreateMoveRequest{ false };
		do {
			EPathFollowingRequestResult::Type requestResult{ enemyAI->MoveToLocation(UNavigationSystem::GetRandomPointInNavigableRadius(enemyAI, GetCenterPoint(OwnerComp), Radius, GetMainNavData())) };
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

FVector UBTMoveToRandomPositionTask::GetCenterPoint(const UBehaviorTreeComponent & OwnerComp) const
{
	const UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();
	if (!MoveAroundSelf && blackboard) {
		TSubclassOf<UBlackboardKeyType> keyType = blackboard->GetKeyType(blackboard->GetKeyID(Center.SelectedKeyName));
		if (keyType == UBlackboardKeyType_Vector::StaticClass()) {
			return blackboard->GetValue<UBlackboardKeyType_Vector>(Center.SelectedKeyName);
		}
		else if (keyType == UBlackboardKeyType_Object::StaticClass()) {
			AActor *actor = Cast<AActor>(blackboard->GetValue<UBlackboardKeyType_Object>(Center.SelectedKeyName));
			if (actor) {
				return actor->GetActorLocation();
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Unable to find center point, using self location instead."));
		}
	}
	else if (!blackboard) {
		UE_LOG(LogTemp, Error, TEXT("Missing blackboard!"));
	}

	// If no other return paths has occured, do this.
	AEnemyAI *enemyAI = Cast<AEnemyAI>(OwnerComp.GetAIOwner());
	if (enemyAI && enemyAI->GetPossessedPawn()) {
		return enemyAI->GetPossessedPawn()->GetActorLocation();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Some kind of error happened!"));
		return FVector{ 0.f, 0.f, 0.f };
	}
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
