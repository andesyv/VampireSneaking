// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacterBase.h"
#include "Enemy.h"
#include "PlayerVamp.generated.h"

UCLASS()
class VAMPIRESNEAKING_API APlayerVamp : public APlayableCharacterBase
{
	GENERATED_BODY()

private:
	/**
	* Is there an enemy in front of the player who is affected by the blood suck?
	* NB: Don't change this directly, should only be changed via SetEnemyLocked.
	*/
	bool EnemyLocked{ false };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// What enemy is being sucked.
	AEnemy *suckedEnemy{ nullptr };
	
	// Returns whether there is an enemy in front of the player.
	bool EnemyInFront();

	// The actual sucking of the blood.
	void SuckBlood(float amount, float DeltaTime);

	// Setter for EnemyLocked. Use this to set EnemyLocked.
	const bool SetEnemyLocked(bool state);

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
};
