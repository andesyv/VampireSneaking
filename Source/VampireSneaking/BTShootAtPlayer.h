// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTShootAtPlayer.generated.h"

/**
 * Shoots at the player.
 */
UCLASS(Blueprintable, DisplayName = "Shoot at player")
class VAMPIRESNEAKING_API UBTShootAtPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
	// Called when task is executed.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** ticks this task
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
protected:
	float timer{ 0 };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Shoot(UBehaviorTreeComponent *OwnerComp);

public:
	// Target to hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetActor;
	
	// Time it takes to shoot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootTime = 1.f;
	
	// Damage the AI deals.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 30.f;
};
