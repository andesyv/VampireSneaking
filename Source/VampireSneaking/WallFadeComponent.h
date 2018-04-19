// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallFadeComponent.generated.h"

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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VAMPIRESNEAKING_API UWallFadeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWallFadeComponent();
		
protected:
	// Called when the game starts
	void BeginPlay() override;

	const FName ParameterName{"EffectValue"};

	// Dynamic material reference
	UMaterialInstanceDynamic *DynamicMaterial{ nullptr };

	// Current fade value
	float fadeValue{ 0.f };

	// The current state. (Fading, showing or finished)
	FadeState CurrentFadeStatus;

	// Ticking function for material visibility.
	void TickMaterial(float DeltaTime);

public:
	// Called every frame
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Fade/show speed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FadeSpeed = 1.f;

	// Sets the visibility for the material.
	void SetMaterialVisible(bool Visibility);
};
