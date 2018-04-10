// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wall.generated.h"

// Forward declarations
class UMaterialInstanceDynamic;
class UMaterialInterface;

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

	// Dynamic material reference
	UMaterialInstanceDynamic *DynamicMaterial{nullptr};

	// Current fade value
	float fadeValue{1.f};

	// The current state. (Fading, showing or finished)
	FadeState CurrentFadeStatus;

public:	
	// Called every frame
	void Tick(float DeltaTime) override;

	// Dynamic material reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	// Fade/show speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeSpeed = 1.f;

	void SetMaterialVisible(bool Visibility);
	
	void TickMaterial(float DeltaTime);
};
