// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacterBase.h"/*
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"*/
#include "PlayerVamp.generated.h"

UCLASS()
class VAMPIRESNEAKING_API APlayerVamp : public APlayableCharacterBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Health
	UPROPERTY(BlueprintGetter = GetHealth)
	float Health = 100.f;

	// Max health, and starting health.
	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetMaxHealth)
	float MaxHealth = 100.f;

	bool ded{ false };

public:	
	// Sets default values for this pawn's properties
	APlayerVamp(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent *collider = nullptr;

	// Add extra rotation to the rotation of the character.
	UPROPERTY(EditAnywhere)
	FRotator AdjustmentRotation = FRotator{ 0.f, 0.f, 0.f };
};
