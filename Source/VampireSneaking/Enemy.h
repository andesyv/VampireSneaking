// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Enemy.generated.h"

// Forward declarations
class ATargetPoint;
class UHealthComponent;

UCLASS()
class VAMPIRESNEAKING_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

	// Health and blood component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthComponent *HealthComponent = nullptr;

	// Damage the enemy deals
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamage = 30.f;

	/** Spawns a bullet.
	 * To be used with shooting animation.
	 * @param BulletOrientation		The orientation of the bullet. x direction is the forward direction.
	 * @param SpawnPosition			The position to spawn the bullet. Defaults to position of enemy.
	 * @param Damage				Amount of damage the bullet will deal.
	 * @param BulletOwner					The owner of the bullet.
	 */
	UFUNCTION(BlueprintCallable)
	void SpawnBullet(FRotator BulletOrientation, FVector SpawnPosition = FVector::ZeroVector, float Damage = 30.f, AActor* BulletOwner = nullptr) const;

	// Blueprint helper function that shoots at the player.
	UFUNCTION(BlueprintCallable)
	void ShootAtPlayer();

	/** Melee attack for enemy
	 * To be used on melee enemies.
	 */
	UFUNCTION(BlueprintCallable)
	void MeleeAttack(float Damage = 30.f, float AttackRange = 10.f, float AttackAngle = 30.f);

	// Blueprint helper function that does a melee attack towards the player.
	UFUNCTION(BlueprintCallable)
	void AttackPlayer();

	// Projectile class
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AEnemyBullet> ProjectileClass;

	/** Apply damage to this actor.
	* @see https://www.unrealengine.com/blog/damage-in-ue4
	* @param DamageAmount		How much damage to apply
	* @param DamageEvent		Data package that fully describes the damage received.
	* @param EventInstigator	The Controller responsible for the damage.
	* @param DamageCauser		The Actor that directly caused the damage (e.g. the projectile that exploded, the rock that landed on you)
	* @return					The amount of damage actually applied.
	*/
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Behavior tree for Enemy. Defines the kind of behavior the Enemy is going to have.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Logic")
	class UBehaviorTree *BehaviorTree;

	// Patrol points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Logic")
	TArray<ATargetPoint*> PatrolPoints;

	// Explosion damage reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UDamageType_Explosion> ExplosionDamage;

	bool beingSucked{ false };
};
