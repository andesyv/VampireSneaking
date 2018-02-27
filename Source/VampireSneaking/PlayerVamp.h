// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayableCharacterBase.h"
#include "PlayerVamp.generated.h"

UCLASS()
class VAMPIRESNEAKING_API APlayerVamp : public APlayableCharacterBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this pawn's properties
	APlayerVamp(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent *collider = nullptr;

	// Add extra rotation to the rotation of the character.
	UPROPERTY(EditAnywhere)
	FRotator AdjustmentRotation = FRotator{ 0.f, 0.f, 0.f };
};
