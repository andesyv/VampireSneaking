// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "AIVisionCheck.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API UAIVisionCheck : public UBTService
{
	GENERATED_BODY()
	
private:
	// This happens every tick.
	virtual void TickNode (UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) override;
	

};
