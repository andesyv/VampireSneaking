// Fill out your copyright notice in the Description page of Project Settings.

#include "BTSetBool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

EBTNodeResult::Type UBTSetBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();
	if (blackboard && !(blackboard->SetValue<UBlackboardKeyType_Bool>(BoolKey.SelectedKeyName, Value))) {
		UE_LOG(LogTemp, Error, TEXT("Failed to set blackboard bool."));
		return EBTNodeResult::Failed;
	}
    return EBTNodeResult::Succeeded;
}


