// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


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
	/*
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	*/

	


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
