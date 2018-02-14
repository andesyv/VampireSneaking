// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAI.h"
#include "Enemy.h"
#include "Engine/TargetPoint.h"

AEnemyAI::AEnemyAI() {

	// Making a default blackboard and behavior tree. (A behavior tree inherits from BrainComponent)
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));

	PointArr.Add(FName{ "Point1" });
	PointArr.Add(FName{ "Point2" });
}

void AEnemyAI::BeginPlay() {
	UWorld *world = GetWorld();
	TArray<FVector> positions{ FVector{0.f, 0.f, 0.f}, FVector{100.f, 100.f, 0.f} };
	if (world) {
		for (int i{ 0 }; i < PointArr.Num(); ++i) {
			ATargetPoint *point = world->SpawnActor<ATargetPoint>(positions[i], FRotator::ZeroRotator);
			if (point) {
				Blackboard->SetValueAsObject(PointArr[i], point);
			}
		}
	}
}

void AEnemyAI::Possess(APawn *Pawn) {

	Super::Possess(Pawn);

	UE_LOG(LogTemp, Warning, TEXT("I am possessed!!"))

	// Setup blackboard and behavior tree.
	UBehaviorTreeComponent *behaviorTree = Cast<UBehaviorTreeComponent>(BrainComponent);
	AEnemy *possessedEnemy = Cast<AEnemy>(Pawn);
	if (possessedEnemy && behaviorTree) {
		
		if (possessedEnemy->BehaviorTree->BlackboardAsset) {
			Blackboard->InitializeBlackboard(*possessedEnemy->BehaviorTree->BlackboardAsset);
		}

		behaviorTree->StartTree(*possessedEnemy->BehaviorTree);
	}
}

