// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/EnemyBullet.h"
#include "BTShootAtPlayer.generated.h"

/**
 * Shoots at the player.
 */
UCLASS(Blueprintable, DisplayName = "Shoot at player")
class VAMPIRESNEAKING_API UBTShootAtPlayer : public UBTTaskNode
{
	GENERATED_BODY()

	// Called when task is executed.
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** ticks this task
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	float timer{ 0 };

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Shoot(UBehaviorTreeComponent *OwnerComp);

	void PlayExplotion(AActor *enemy, AActor *player);

	void SpawnBullet(UBehaviorTreeComponent* OwnerComp, FRotator BulletOrientation);

public:
	// Target to hit.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetActor;

	/**
	 * Bool
	 * If the task should reset the cooldown when executed.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector ResetAttackCooldown;

	// Time it takes to shoot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootTime = 1.f;
	
	// Damage the AI deals.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 30.f;

	// Explotion effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem *ExplotionFX = nullptr;

	// Projectile class
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyBullet> ProjectileClass;
};
