// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyAI.h"
#include "Enemy.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Engine/TargetPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/PlayableCharacterBase.h"
#include "AI/AIVisionCheck.h"
#include "TimerManager.h"

AEnemyAI::AEnemyAI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Making a default blackboard and behavior tree. (A behavior tree inherits from BrainComponent)
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));

	// Assign to Team 1
    SetGenericTeamId(FGenericTeamId(1));
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

	// Fetch reference to behaviorTreeComponent and enemy.
	UBehaviorTreeComponent *behaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	AEnemy *possessedEnemy = Cast<AEnemy>(Pawn);

	// Setup blackboard and behavior tree.
	if (possessedEnemy && behaviorTreeComp) {
		if (possessedEnemy->BehaviorTree) {
			if (possessedEnemy->BehaviorTree->BlackboardAsset) {
				
				// Setup blackboard.
				Blackboard->InitializeBlackboard(*possessedEnemy->BehaviorTree->BlackboardAsset);

				// Setup behavior tree.
				behaviorTreeComp->StartTree(*possessedEnemy->BehaviorTree);

				// Setup first control point.
				if (!Blackboard->SetValue<UBlackboardKeyType_Vector>(TEXT("NextPoint"), Pawn->GetActorLocation())) {
					UE_LOG(LogTemp, Warning, TEXT("Failed to set start patrol point for EnemyAI: %s"), *GetName());
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Enemy behavior tree is missing a blackboard!"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Enemy is missing behavior tree!"));
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("Missing either something to possess or a behavior tree!"));
	}
}

void AEnemyAI::BeginPlay() {
	Super::BeginPlay();

	TArray<UAIPerceptionComponent*> comps{};
	GetComponents<UAIPerceptionComponent>(comps);
	for (auto item : comps) {
		if (item) {
			AIPerceptionComp = item;
		}
	}
	
	if (GetPerceptionComp()) {
		GetPerceptionComp()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAI::UpdateState);
	}
}

UAIPerceptionComponent* const AEnemyAI::GetPerceptionComp() {
	if (AIPerceptionComp) {
		return AIPerceptionComp;
	} else {
		return nullptr;
	}
}

void AEnemyAI::UnPossess()
{
	Super::UnPossess();

}

void AEnemyAI::SetAIIdleState() {
	if (Blackboard) {
		if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(AIState::Idle))) {
			UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Missing blackboard!."));
	}
}

void AEnemyAI::UpdateState(TArray<AActor*> UpdatedActors) {
	for (auto item : UpdatedActors) {
		if (Cast<APlayableCharacterBase>(item) == nullptr) {
			continue;
		}

		FActorPerceptionBlueprintInfo perceivedInfo;
		if (item && GetPerceptionComp() && GetPerceptionComp()->GetActorsPerception(item, perceivedInfo)) {
			if (perceivedInfo.LastSensedStimuli.Num() > 0) {
				if (Blackboard) {
					if (GetWorld() && SearchingTimerHandle.IsValid()) {
						GetWorld()->GetTimerManager().ClearTimer(SearchingTimerHandle);
					}

					if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>((perceivedInfo.LastSensedStimuli[0].WasSuccessfullySensed()) ? AIState::Combat : AIState::Searching))) {
						UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
					}

					if (!Blackboard->SetValue<UBlackboardKeyType_Object>(TEXT("TargetActor"), (perceivedInfo.LastSensedStimuli[0].WasSuccessfullySensed()) ? item : nullptr)) {
						UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard TargetActor."));
					}

					if (!perceivedInfo.LastSensedStimuli[0].WasSuccessfullySensed()) {
						if (!Blackboard->SetValue<UBlackboardKeyType_Vector>(TEXT("LastSeenPosition"), item->GetActorLocation())) {
							UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard LastSeenPosition."));
						}

						if (GetWorld()) {
							GetWorld()->GetTimerManager().SetTimer(SearchingTimerHandle, this, &AEnemyAI::SetAIIdleState, 3.f);
						}
					}
				} else {
					UE_LOG(LogTemp, Warning, TEXT("Missing blackboard!."));
				}
			}
		}
	}
}