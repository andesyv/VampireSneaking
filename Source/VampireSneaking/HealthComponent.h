// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VAMPIRESNEAKING_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:




	






protected:

	// Current and starting blood.
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetBlood, Category = "Blood")
		float Blood = 50.f;

	// Maximum amount of blood. (Will later be updated to have an indefinite amount of blood)
	UPROPERTY(BlueprintGetter = GetMaxBlood, Category = "Blood")
		float MaxBlood = 100.f;

	// Make the maximum amount of blood be the starting blood?
	UPROPERTY(EditDefaultsOnly, Category = "Blood")
		bool MaxBloodIsStartingBlood = false;

	// Should the player/enemy use no blood on abilities? (Cheat)
	UPROPERTY(EditDefaultsOnly, Category = "Blood")
		bool CHEAT_InfiniteBlood = false;

	// If the player/enemy is out of blood.
	UPROPERTY(BlueprintGetter = IsOutOfBlood, Category = "Blood")
		bool OutOfBlood = false;

	// Should the player/enemy die when they run out of blood?
	UPROPERTY(EditDefaultsOnly, Category = "Blood")
		bool DieWhenOutOfBlood = false;


	// Should the player/enemy take no damage? (Cheat)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool CHEAT_Godmode = false;

	// Function called when the player/enemy dies
	UFUNCTION()
		void Die();

	// Iz ded
	bool ded{ false };
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

	// Returns true if the player/enemy is out of blood.
	UFUNCTION(BlueprintGetter, Category = "Blood")
		const bool IsOutOfBlood() const;

	
	// Take damage.
	UFUNCTION(BlueprintCallable, Category = "Health")
		const float TakeDamage(float amount);

	// Delegate called on death.
	UPROPERTY(BlueprintAssignable, Category = "Health")
		FDeathEvent OnDeath;
};
