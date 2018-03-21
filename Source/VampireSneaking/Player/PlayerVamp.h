// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayableCharacterBase.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerVamp.generated.h"

// Forward declarations
class AEnemy;
class ACustomPlayerController;

UCLASS()
class VAMPIRESNEAKING_API APlayerVamp : public APlayableCharacterBase, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Currently sucking blood?
	bool SuckingBlood{ false };

	// What enemy is being sucked.
	AEnemy *suckedEnemy{ nullptr };
	
	// Returns whether there is an enemy in front of the player.
	bool EnemyInFront();

	// The actual sucking of the blood.
	void SuckBlood(float amount, float DeltaTime);

	// Internal toggle of bloodsucking.
	bool ToggleBloodSucking();

	void Attack();

	FGenericTeamId TeamId;
 
    virtual FGenericTeamId GetGenericTeamId() const override;

public:	
	// Sets default values for this pawn's properties
	APlayerVamp(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Speed of blood sucking (blood/second).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuckSpeed = 5.f;

	// Damage type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

	// Amount of damage on attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDamage = 30.f;

	// Range of attack from player center
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange = 100.f;

	// Wideness of attack in angle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackAngle = 30.f;

	// Hit force of normal attack.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float HitForce = 10.f;
};
