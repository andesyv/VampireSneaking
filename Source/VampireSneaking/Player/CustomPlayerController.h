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

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;
	
public:

	void SwapActorLocation(AActor *first, AActor *second);




	///////////////////////////////////////////////////////////////////////////////
	// Health
protected:

	// Health
	UPROPERTY(BlueprintGetter = GetHealth, Category="Health")
		float Health = 100.f;

	// Max health, and starting health.
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxHealth, Category = "Health")
		float MaxHealth = 100.f;

	bool ded{ false };

public:
	// Getter for Health
	UFUNCTION(BlueprintGetter, Category = "Health")
		const float GetHealth() const;

	// Getter for MaxHealth
	UFUNCTION(BlueprintGetter, Category = "Health")
		const float GetMaxHealth() const;

	// Get Health in percentage
	UFUNCTION(BlueprintCallable, Category = "Health")
		const float GetPercentageHealth() const;

	// Take damage.
	UFUNCTION(BlueprintCallable, Category = "Health")
		const float TakeDamage(float damage);




	///////////////////////////////////////////////////////////////////////////
	// Blood
protected:

	// Blood
	UPROPERTY(BlueprintGetter = GetBlood, Category = "Blood")
		float Blood = 50.f;

	// Maximum amount of blood. (Will later be updated to have an indefinite amount of blood)
	UPROPERTY(BlueprintGetter = GetMaxBlood, Category = "Blood")
		float MaxBlood = 100.f;

	// If the player is out of blood.
	UPROPERTY(BlueprintGetter = IsOutOfBlood, Category = "Blood")
		bool OutOfBlood = false;

public:
	// Getter for Blood
	UFUNCTION(BlueprintGetter, Category = "Blood")
		const float GetBlood() const;

	// Getter for MaxBlood
	UFUNCTION(BlueprintGetter, Category = "Blood")
		const float GetMaxBlood() const;

	// Get percentage amount of blood.
	UFUNCTION(BlueprintCallable, Category = "Blood")
		const float GetPercentageBlood() const;

	// Adds blood to the current amount of blood.
	UFUNCTION(BlueprintCallable, Category = "Blood")
		const float AddBlood(float amount);

	// Returns true if the player is out of blood.
	UFUNCTION(BlueprintGetter, Category = "Blood")
		const bool IsOutOfBlood() const;
};
