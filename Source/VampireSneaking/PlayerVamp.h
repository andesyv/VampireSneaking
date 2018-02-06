// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PlayerVamp.generated.h"

UCLASS()
class VAMPIRESNEAKING_API APlayerVamp : public APawn
{
	GENERATED_BODY()

private:
	void MoveX(float amount);
	void MoveY(float amount);
	void Rotate();

	FVector velocity{ 0, 0, 0 };

	APlayerController *controller;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this pawn's properties
	APlayerVamp();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent *mesh = nullptr;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent *collider = nullptr;

	// Movement speed of character.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 10.f;

	// Add extra rotation to the rotation of the character.
	UPROPERTY(EditAnywhere)
	FRotator AdjustmentRotation = FRotator{ 0, 0, 0 };
};
