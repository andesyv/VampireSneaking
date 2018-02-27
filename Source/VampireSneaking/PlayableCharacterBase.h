// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "CustomPlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayableCharacterBase.generated.h"

/**
* Character base for all characters that the player should be able to control.
*/
UCLASS(Abstract, meta = (DisplayName="Playable Character Base"))
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

	// Rotation after mouse cursor.
	virtual void Rotate();

	// Playercontroller reference.
	ACustomPlayerController *controller;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Extra rotation added to the mouse rotation. Get's set in BeginPlay, do not change in runtime.
	FRotator meshStartRotation{ 0.f, 0.f, 0.f };

	// Pointer to the skeletalmesh-component.
	USkeletalMeshComponent *meshComponent = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera component
	UPROPERTY(VisibleAnywhere)
	UCameraComponent *Camera = nullptr;

	// Helper function to get the forward vector of the mesh, relative to the mesh's rotation.
	UFUNCTION(BlueprintCallable)
	FVector GetMeshForwardVector() const;




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
};
