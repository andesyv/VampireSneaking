// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "PlayableCharacterBase.generated.h"

// Forward declarations
class ACustomPlayerController;
class UMeshComponent;

/**
* Character base for all characters that the player should be able to control.
* (Pretty useless after I changed it to only be 1 character that the player should be able to control..)
*/
UCLASS(Abstract, meta = (DisplayName="Playable Character Base"))
class VAMPIRESNEAKING_API APlayableCharacterBase : public ACharacter, public IGenericTeamAgentInterface
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
	void BeginPlay() override;

	// Extra rotation added to the mouse rotation. Get's set in BeginPlay, do not change in runtime.
	FRotator meshStartRotation{ 0.f, 0.f, 0.f };

	// Pointer to the skeletalmesh-component.
	UMeshComponent *meshComponent = nullptr;

	// Time before the player can attack again.
	float TimeBeforeNextAttack{ 0.f };

	// Time before the player can dash again.
	float TimeBeforeNextDash{ 0.f };

	// Team functionality
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const override;

	// Currently sucking blood?
	bool SuckingBlood{ false };

	// Is the player dead?
	UPROPERTY(BlueprintGetter = IsPlayerDead)
	bool dead = false;

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Helper function to get the forward vector of the mesh, relative to the mesh's rotation.
	UFUNCTION(BlueprintCallable)
	FVector GetMeshForwardVector() const;

	// Helper function to get the vector from the player to the mouse. z is neutralized.
	UFUNCTION(BlueprintCallable)
	FVector GetMouseVector() const;

	// Returns the rotation matrix to rotate vector a to b
	UFUNCTION(BlueprintCallable)
	FRotator RotateAToB(FVector a, FVector b) const;

	// Returns true if the player is sucking blood
	UFUNCTION(BlueprintCallable)
	bool IsSuckingBlood() const;

	// Returns true if the player is dead
	UFUNCTION(BlueprintCallable, BlueprintGetter)
	bool IsPlayerDead() const;
};
