// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Engine/TargetPoint.h"
#include "Enemy.generated.h"

UCLASS()
class VAMPIRESNEAKING_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Behavior tree for Enemy. Defines the kind of behavior the Enemy is going to have.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Logic")
	class UBehaviorTree *BehaviorTree;

	// The angle from the center that the AI will be able to see, in degrees. Total angle that the AI can see is this * 2.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Logic")
	float VisionAngle = 10.f;

	// The length the AI will be able to see.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Logic")
	float VisionRadius = 100.f;

	// Patrol points
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Logic")
	TArray<ATargetPoint*> PatrolPoints;
};
