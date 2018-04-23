// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayableCharacterBase.h"
#include "Engine/World.h"
#include "PlayerVamp.generated.h"

// Forward declarations
class AEnemy;

UCLASS(Blueprintable, meta = (DisplayName = "Player Vampire"))
class VAMPIRESNEAKING_API APlayerVamp : public APlayableCharacterBase
{
	GENERATED_BODY()

protected:
	// What enemy is being sucked.
	AEnemy *suckedEnemy{ nullptr };

	// Returns whether there is an enemy in front of the player.
	bool EnemyInFront();

	// The actual sucking of the blood.
	void SuckBlood(float amount, float DeltaTime);

	// Internal toggle of bloodsucking.
	bool ToggleBloodSucking() const;

	// Check if the attack is off cooldown, and if so run attack
	UFUNCTION(BlueprintCallable)
	void AttackCheck();

	// Must've calculated this wrong, cause this shit DOESN'T WORK!
	FVector BallisticTrajectory(const FVector &EndPoint);

	// Player attack function.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack();

	void BloodAttack();

public:
	// Sets default values for this pawn's properties
	APlayerVamp(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Speed of blood sucking (blood/second).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuckSpeed = 15.f;

	// Damage type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

	// Amount of damage on attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = ( DisplayName = "Damage"))
	float AttackDamage = 30.f;

	// Range of attack from player center
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (DisplayName = "Range"))
	float AttackRange = 100.f;

	// Wideness of attack in angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (DisplayName = "Angle"))
	float AttackAngle = 30.f;

	// Hit force of normal attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (DisplayName = "Hit Force"))
	float HitForce = 10.f;

	// Cooldown before the player can attack again.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (DisplayName = "Cooldown"))
	float AttackCooldown = 1.5f;

	// Blood projectile class.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blood projectile", meta = (DisplayName = "Projectile Class"))
	TSubclassOf<class AProjectile> ProjectileBlueprint ;

	// The activation cost of the blood projectile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blood projectile", meta = ( DisplayName = "Activation Cost"))
	float BloodProjectileActivationCost = 30.f;
	
	UFUNCTION()
		void Dash(); 
	
};
