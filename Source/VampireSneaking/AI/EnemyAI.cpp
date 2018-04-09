// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyAI.h"
#include "Enemy.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Player/PlayableCharacterBase.h"
#include "TimerManager.h"
#include "HealthComponent.h"

AEnemyAI::AEnemyAI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Making a default blackboard and behavior tree. (A behavior tree inherits from BrainComponent)
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));

	// Make health component.
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));

	// Add death event.
	if (HealthComponent) {
		HealthComponent->OnDeath.AddDynamic(this, &AEnemyAI::Death);
	}

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

	// Stop behavior tree.
	UBehaviorTreeComponent *behaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (behaviorTreeComp) {
		behaviorTreeComp->StopTree();
	}
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

void AEnemyAI::UpdateState(const TArray<AActor*> &UpdatedActors) {
	for (auto item : UpdatedActors) {
		if (!item->IsA(APlayableCharacterBase::StaticClass())) {
			continue;
		}

		FActorPerceptionBlueprintInfo perceivedInfo;
		if (item && GetPerceptionComp() && GetPerceptionComp()->GetActorsPerception(item, perceivedInfo)) {
			if (perceivedInfo.LastSensedStimuli.Num() > 0) {
				if (Blackboard) {
					if (GetWorld() && SearchingTimerHandle.IsValid()) {
						GetWorld()->GetTimerManager().ClearTimer(SearchingTimerHandle);
					}

					if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>((perceivedInfo.LastSensedStimuli[0].WasSuccessfullySensed())
					? AIState::Combat : AIState::Searching))) {
						UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
					}

					if (!Blackboard->SetValue<UBlackboardKeyType_Object>(TEXT("TargetActor"), (perceivedInfo.LastSensedStimuli[0].WasSuccessfullySensed())
					? item : nullptr)) {
						UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard TargetActor."));
					}

					if (!perceivedInfo.LastSensedStimuli[0].WasSuccessfullySensed()) {
						if (!Blackboard->SetValue<UBlackboardKeyType_Vector>(TEXT("LastSeenPosition"), item->GetActorLocation())) {
							UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard LastSeenPosition."));
						}

						if (GetWorld()) {
							GetWorld()->GetTimerManager().SetTimer(SearchingTimerHandle, this, &AEnemyAI::SetAIIdleState, SearchTime);
						}
					}
				} else {
					UE_LOG(LogTemp, Warning, TEXT("Missing blackboard!."));
				}
			}
		}
	}
}

bool AEnemyAI::ToggleSucking() {
	if (GetWorld() && SearchingTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(SearchingTimerHandle);
	}

	if (Blackboard) {
		if (static_cast<AIState>(Blackboard->GetValue<UBlackboardKeyType_Enum>(TEXT("State"))) != AIState::Combat) {
			bool StateIsFrozen{ lastState != AIState::Frozen && static_cast<AIState>(Blackboard->GetValue<UBlackboardKeyType_Enum>(TEXT("State"))) == AIState::Frozen };
			if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(StateIsFrozen ? AIState::Idle : AIState::Frozen))) {
				UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
				return false;
			}
			return !StateIsFrozen;
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Missing blackboard!."));
		return false;
	}
	return false;
}

void AEnemyAI::Death_Implementation() {
	APawn *enemy = GetPawn();
	if (enemy) {
		UnPossess();
		enemy->Destroy();
	}
	Destroy();
}