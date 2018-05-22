// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, DisplayName = "Health Component") )
class VAMPIRESNEAKING_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	void BeginPlay() override;

	// Current blood.
	UPROPERTY(BlueprintGetter = GetBlood, Category = "Blood")
		float Blood = 100.f;

	// Maximum amount of blood. (Will later be updated to have an indefinite amount of blood)
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxBlood, Category = "Blood")
		float MaxBlood = 100.f;

	// The amount of blood to start with.
	UPROPERTY(EditDefaultsOnly, Category = "Blood")
		float StartingBlood = 80.f;

	// Make the maximum amount of blood be the starting blood?
	UPROPERTY(EditDefaultsOnly, Category = "Blood")
		bool MaxBloodIsStartingBlood = false;

	// Can the player/enemy be drained of blood?
	UPROPERTY(EditAnywhere, BlueprintGetter = CanBeDrained, Category = "Blood", meta = (DisplayName = "Can be drained"))
		bool Suckable = true;

	// If the player/enemy is out of blood.
	UPROPERTY(BlueprintGetter = IsOutOfBlood, Category = "Blood")
		bool OutOfBlood = false;

	// Should the player/enemy take no damage? (Cheat)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blood")
		bool CHEAT_Godmode = false;

	// Function called when the player/enemy dies
	UFUNCTION()
		void Die();

	// Iz ded
	bool ded{ false };
public:
	// Getter for Blood
	UFUNCTION(BlueprintGetter, Category = "Blood")
		float GetBlood() const;

	// Getter for MaxBlood
	UFUNCTION(BlueprintGetter, Category = "Blood")
		float GetMaxBlood() const;

	// Get percentage amount of blood.
	UFUNCTION(BlueprintCallable, Category = "Blood")
		float GetPercentageBlood() const;

	// Adds blood to the current amount of blood.
	UFUNCTION(BlueprintCallable, Category = "Blood")
		float AddBlood(float amount);

	// Returns true if the player/enemy is out of blood.
	UFUNCTION(BlueprintGetter, Category = "Blood")
		bool IsOutOfBlood() const;

	// Returns true if the enemy/player can be drained of blood
	UFUNCTION(BlueprintGetter, Category = "Blood")
	bool CanBeDrained() const;
	
	// Take damage.
	UFUNCTION(BlueprintCallable, Category = "Blood")
		float TakeDamage(float amount);

	// Delegate called on death.
	UPROPERTY(BlueprintAssignable, Category = "Blood")
		FDeathEvent OnDeath;

	// Reset health/blood to starting health/blood
	UFUNCTION(BlueprintCallable, Category = "Blood")
		void Reset();
};
