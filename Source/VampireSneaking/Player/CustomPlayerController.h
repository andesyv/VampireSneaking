// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

	friend class AVampireSneakingGameModeBase;
	friend class ABatMode;

protected:
	virtual void BeginPlay() override;

	// Shall only be changed in the ChangePawn function.
	uint32 CurrentIndex{ 0 };

	TArray<APawn*> ControllablePawns{};

	void ChangePawn();
	void ChangePawn(int index);

	void MoveController(int index);

	bool TransferStats(APawn *newPawn, APawn *oldPawn);

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;
	
public:

	void SwapActorLocation(AActor *first, AActor *second);




	///////////////////////////////////////////////////////////////////////////////
	// Health
protected:

	// Health
	UPROPERTY(BlueprintGetter = GetHealth)
		float Health = 100.f;

	// Max health, and starting health.
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxHealth)
		float MaxHealth = 100.f;

	bool ded{ false };

public:
	// Getter for Health
	UFUNCTION(BlueprintGetter)
		const float GetHealth() const;

	// Getter for MaxHealth
	UFUNCTION(BlueprintGetter)
		const float GetMaxHealth() const;

	// Get Health in percentage
	UFUNCTION(BlueprintCallable)
		const float GetPercentageHealth() const;

	// Take damage.
	UFUNCTION(BlueprintCallable)
		const float TakeDamage(float damage);




	///////////////////////////////////////////////////////////////////////////
	// Blood
protected:

	// Blood
	UPROPERTY(BlueprintGetter = GetBlood)
		float Blood = 50.f;

	// Maximum amount of blood. (Will later be updated to have an indefinite amount of blood)
	UPROPERTY(BlueprintGetter = GetMaxBlood)
		float MaxBlood = 100.f;

	// If the player is out of blood.
	UPROPERTY(BlueprintGetter = IsOutOfBlood)
		bool OutOfBlood = false;

public:
	// Getter for Blood
	UFUNCTION(BlueprintGetter)
		const float GetBlood() const;

	// Getter for MaxBlood
	UFUNCTION(BlueprintGetter)
		const float GetMaxBlood() const;

	// Get percentage amount of blood.
	UFUNCTION(BlueprintCallable)
		const float GetPercentageBlood() const;

	// Adds blood to the current amount of blood.
	UFUNCTION(BlueprintCallable)
		const float AddBlood(float amount);

	// Returns true if the player is out of blood.
	UFUNCTION(BlueprintGetter)
		const bool IsOutOfBlood() const;
};
