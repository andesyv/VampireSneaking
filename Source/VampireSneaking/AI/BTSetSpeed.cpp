// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSetSpeed.h"
#include "Enemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

EBTNodeResult::Type UBTSetSpeed::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AEnemy *enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemy && enemy->GetMovementComponent() && Cast<UCharacterMovementComponent>(enemy->GetMovementComponent())) {
		Cast<UCharacterMovementComponent>(enemy->GetMovementComponent())->MaxWalkSpeed = Speed;
		return EBTNodeResult::Succeeded;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Cannot retrieve enemy's movement component."));
		return EBTNodeResult::Failed;
	}
}
