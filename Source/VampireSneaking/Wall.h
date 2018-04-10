// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

// Forward declarations
class UMaterialInstanceDynamic;
class UStaticMeshComponent;

// Fade state
enum class FadeState
{
	Finished,
	Fading,
	Showing,
};

UCLASS()
class VAMPIRESNEAKING_API AWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWall();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Mesh component to the wall.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *CubeMesh;

	// Dynamic material reference
	UMaterialInstanceDynamic *DynamicMaterial{nullptr};

	// Current fade value
	float fadeValue{0.f};

	// The current state. (Fading, showing or finished)
	FadeState CurrentFadeStatus;

	// Ticking function for material visibility.
	void TickMaterial(float DeltaTime);

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

	// Fade/show speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeSpeed = 1.f;

	// Sets the visibility for the material.
	void SetMaterialVisible(bool Visibility);
};
