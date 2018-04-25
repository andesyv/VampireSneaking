// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayableCharacterBase.h"
#include "Engine/World.h"
#include "PlayerVamp.generated.h"

// Forward declarations
class AEnemy;
class UParticleSystemComponent;

UCLASS(Blueprintable, meta = (DisplayName = "Player Vampire"))
class VAMPIRESNEAKING_API APlayerVamp : public APlayableCharacterBase
{
	GENERATED_BODY()

private:

	// Internal timer for when the player will be able to initiate into batmode.
	float TimeBeforeNextBatToggle = 0.f;

protected:
	// Particle system
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UParticleSystemComponent * ParticleSystem;

	// What enemy is being sucked.
	AEnemy *suckedEnemy{ nullptr };

	// Returns whether there is an enemy in front of the player.
	bool EnemyInFront();

	// The actual sucking of the blood.
	void SuckBlood(float amount, float DeltaTime);

	// Internal toggle of bloodsucking.
	bool ToggleBloodSucking() const;

	// Must've calculated this wrong, cause this shit DOESN'T WORK!
	FVector BallisticTrajectory(const FVector &EndPoint);
	
	// Check if the attack is off cooldown, and if so run attack
	UFUNCTION(BlueprintCallable)
	void AttackCheck();

	// Player attack function.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Attack();

	void BloodAttack();

	// Initiates the transformation.
	void BatModeToggle();

	// Is the player currently in batmode?
	bool BatMode{ false };

	// Is the player currently toggling modes?
	bool TogglingModes{ false };

	// The cooldown between batmode and normal mode.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Batmode", meta = (DisplayName = "Cooldown"))
	float BatModeCooldown = 1.f;

	FTimerHandle BatModeTimerHandle;
	// Function for finishing the transformation
	void BatModeFinish();

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
	float AttackRange = 220;

	// Wideness of attack in angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (DisplayName = "Angle"))
	float AttackAngle = 30.f;

	// Hit force of normal attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (DisplayName = "Hit Force"))
	float HitForce = 10.f;

	// Cooldown before the player can attack again.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (DisplayName = "Cooldown"))
	float AttackCooldown = 3.f;
	
	// Blood projectile class.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blood projectile", meta = (DisplayName = "Projectile Class"))
	TSubclassOf<class AProjectile> ProjectileBlueprint ;

	// The activation cost of the blood projectile.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blood projectile", meta = ( DisplayName = "Activation Cost"))
	float BloodProjectileActivationCost = 30.f;
	
	// Distance the dash moves the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = ( DisplayName = "Distance"))
	float DashDistance = 3000.f;

	// Cooldown between dash.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (DisplayName = "Cooldown"))
	float DashCooldown = 1.5f;

	// CHEAT: No dash cooldown
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash", meta = (DisplayName = "CHEAT: No Cooldown"))
	bool CHEAT_NoCooldown = false;

	UFUNCTION()
		void Dash(); 
	
};
