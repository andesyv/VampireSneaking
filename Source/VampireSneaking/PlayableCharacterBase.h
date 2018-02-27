// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "CustomPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayableCharacterBase.generated.h"

UCLASS()
class VAMPIRESNEAKING_API APlayableCharacterBase : public ACharacter
{
	GENERATED_BODY()

	friend class ACustomPlayerController;

public:
	// Sets default values for this character's properties
	APlayableCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void MoveX(float amount);
	virtual void MoveY(float amount);
	virtual void Rotate();

	APlayerController *controller;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FRotator meshStartRotation{ 0.f, 0.f, 0.f };

	USkeletalMeshComponent *meshComponent = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent *Camera = nullptr;




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
};
