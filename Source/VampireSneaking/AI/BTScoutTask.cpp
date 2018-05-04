// Fill out your copyright notice in the Description page of Project Settings.

#include "BTScoutTask.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"

EBTNodeResult::Type UBTScoutTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn()) {
		FRotator newRotation{ OwnerComp.GetAIOwner()->GetPawn()->GetActorRotation() };
		newRotation.Add(0.f, FMath::Sin(UKismetSystemLibrary::GetGameTimeInSeconds(this)) * Amount, 0.f);
		// StartRotation = OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(newRotation);
		OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(newRotation);
	}
	// bNotifyTick = 1;
	/*if (OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn()) {
		StartRotation = OwnerComp.GetAIOwner()->GetPawn()->GetActorRotation();
	}
	timer = 0.f;

	return EBTNodeResult::InProgress;*/
	return EBTNodeResult::Succeeded;
}

void UBTScoutTask::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetOwner() && OwnerComp.GetAIOwner()->GetPawn())
	{
		FRotator newRotation{ StartRotation };
		newRotation.Add(0.f, FMath::Sin(timer * Frequency) * Amount, 0.f);
		OwnerComp.GetAIOwner()->GetPawn()->SetActorRotation(newRotation);

		timer += DeltaSeconds;
	}
}
