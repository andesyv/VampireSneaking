// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FollowCamera.generated.h"

// Forward declarations
class USceneComponent;
class UCameraComponent;

UCLASS()
class VAMPIRESNEAKING_API AFollowCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollowCamera();

	// Target to follow
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* Target = nullptr;

	// Camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera = nullptr;

	//// Update cameras position to the players position?
	//UPROPERTY(EditDefaultsOnly)
	//	bool UpdatePosition = true;

	// Lock z-coordinate?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool LockZ = true;

protected:
	// Root component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root = nullptr;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;	
};
