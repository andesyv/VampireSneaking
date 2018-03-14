// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayableCharacterBase.h"
#include "BatMode.generated.h"

// Forward declarations
class UStaticMeshComponent;

UCLASS()
class VAMPIRESNEAKING_API ABatMode : public APlayableCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABatMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Rotate() override;

public:
	// Mesh for bat.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *batModel = nullptr;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// How fast the ability drains blood in blood/second.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrainSpeed = 10.f;

	// Activation cost for batmode
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ActivationCost = 10.f;

};
