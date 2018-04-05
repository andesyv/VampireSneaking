// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacterBase.generated.h"

// Forward declarations
class USkeletalMeshComponent;
class UCameraComponent;
class ACustomPlayerController;


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

	// Helper function to get the forward vector of the mesh, relative to the mesh's rotation.
	UFUNCTION(BlueprintCallable)
	FVector GetMeshForwardVector() const;


};
