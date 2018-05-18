// Fill out your copyright notice in the Description page of Project Settings.

#include "BTConstantRotation.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

EBTNodeResult::Type UBTConstantRotation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	bNotifyTick = 1;
	return EBTNodeResult::InProgress;
}

void UBTConstantRotation::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn *player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn *enemy = OwnerComp.GetAIOwner()->GetPawn();
	if (player && enemy) {
		FVector toPlayer{ player->GetActorLocation() - enemy->GetActorLocation() };
		toPlayer.Z = 0;
		enemy->SetActorRotation(toPlayer.Rotation());
	}
}
