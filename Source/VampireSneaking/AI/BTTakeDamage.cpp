// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTakeDamage.h"
#include "Player/DamageType_Explosion.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "AI/EnemyAI.h"
#include "HealthComponent.h"

EBTNodeResult::Type UBTTakeDamage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AEnemyAI *enemyAI = Cast<AEnemyAI>(OwnerComp.GetAIOwner());
    if (enemyAI && enemyAI->GetPawn()) {

        /** NOTE:
         * Currently goes to defalt value when the amount specified is too low.
         * Should probs change this in the future.
         */
        if (enemyAI->DamageInfo.FlingAmount <= 1.f) {
            UDamageType_Explosion::HandleDamage(enemyAI->GetPawn(), enemyAI->DamageInfo.FlingDirection);
        } else {
            UDamageType_Explosion::HandleDamage(enemyAI->GetPawn(), enemyAI->DamageInfo.FlingDirection, enemyAI->DamageInfo.FlingAmount);
        }

        if (enemyAI->HealthComponent) {
            if (enemyAI->HealthComponent->TakeDamage(enemyAI->DamageInfo.Damage) < KINDA_SMALL_NUMBER) {
                return EBTNodeResult::Aborted;
            }
        }

        UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();
        if (blackboard) {
            if (!blackboard->SetValue<UBlackboardKeyType_Enum>(State.SelectedKeyName, static_cast<int>(AIState::Combat))) {
			    UE_LOG(LogTemp, Error, TEXT("Failed to set enum state in blackboard!"));
                return EBTNodeResult::Failed;
		    }
            return EBTNodeResult::Succeeded;
        }
    }

    return EBTNodeResult::Failed;
}


