// Fill out your copyright notice in the Description page of Project Settings.

#include "BTShootAtPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "PlayerVamp.h"

EBTNodeResult::Type UBTShootAtPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	timer = 0;
	bNotifyTick = 1;
	return EBTNodeResult::InProgress;
}

void UBTShootAtPlayer::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (timer > ShootTime) {
		Shoot(&OwnerComp);
	}
	timer += DeltaSeconds;
}

void UBTShootAtPlayer::Shoot_Implementation(UBehaviorTreeComponent *OwnerComp)
{
	UBlackboardComponent *blackboard = OwnerComp->GetBlackboardComponent();
	if (blackboard) {
		APlayerVamp *player = Cast<APlayerVamp>(blackboard->GetValue<UBlackboardKeyType_Object>(TargetActor.SelectedKeyName));
		if (player) {
			player->TakeDamage(Damage);
			timer = 0.f;
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Can't receive actor from blackboard!"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't receive blackboard!"));
	}
	FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	return;
}
