// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAI.h"
#include "Enemy.h"
#include "Engine/TargetPoint.h"

AEnemyAI::AEnemyAI() {

	// Making a default blackboard and behavior tree. (A behavior tree inherits from BrainComponent)
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));

	// Adding some debugpoints.
	PointArr.Add(FName{ "Point1" });
	PointArr.Add(FName{ "Point2" });
}

void AEnemyAI::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
	Super::OnMoveCompleted(RequestID, Result);

	UBehaviorTreeComponent *behaviorTree = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (behaviorTree) {
		OnMoveCompletedDelegate.Broadcast(behaviorTree);
	}
}

void AEnemyAI::Possess(APawn *Pawn) {

	Super::Possess(Pawn);

	// UE_LOG(LogTemp, Warning, TEXT("Player controller %s possessed enemy %s!"), *GetName(), *Pawn->GetName());

	possessedPawn = Pawn;

	// Fetch reference to behaviorTreeComponent and enemy.
	UBehaviorTreeComponent *behaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	AEnemy *possessedEnemy = Cast<AEnemy>(Pawn);

	// Setup blackboard and behavior tree.
	if (possessedEnemy && behaviorTreeComp) {
		if (possessedEnemy->BehaviorTree) {
			if (possessedEnemy->BehaviorTree->BlackboardAsset) {
				
				// Setup blackboard.
				if (Blackboard->InitializeBlackboard(*possessedEnemy->BehaviorTree->BlackboardAsset)) {

					// Adding some debugpoints.
					UWorld *world = GetWorld();
					TArray<FVector> positions{ FVector{ 0.f, 0.f, 0.f }, FVector{ 500.f, 500.f, 0.f } };
					if (world) {
						for (int i{ 0 }; i < PointArr.Num(); ++i) {
							ATargetPoint *point = world->SpawnActor<ATargetPoint>(positions[i], FRotator::ZeroRotator);
							if (point) {
								Blackboard->SetValueAsObject(PointArr[i], point);
							}
						}
					}
				}

				// Setup behavior tree.
				behaviorTreeComp->StartTree(*possessedEnemy->BehaviorTree);

			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Enemy behavior tree is missing a blackboard!"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Enemy is missing behavior tree!"));
		}
	}
}

void AEnemyAI::UnPossess()
{
	Super::UnPossess();

	possessedPawn = nullptr;

}

