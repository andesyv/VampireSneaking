// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTSetSpeed.generated.h"

// Forward declarations
class UBehaviorTreeComponent;

/**
 * Sets movement speed of AI.
 */
UCLASS(Blueprintable, DisplayName = "Set Movement Speed")
class VAMPIRESNEAKING_API UBTSetSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	// Movement speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 400.f;

	// Rotate speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateSpeed = 360.f;
};
