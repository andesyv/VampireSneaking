// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageType_Explosion.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API UDamageType_Explosion : public UDamageType
{
	GENERATED_BODY()

public:
	// Handle the damage.
	static void HandleDamage(APawn *HitPawn, const FVector &FlingDirection);
	static void HandleDamage(APawn *HitPawn, const FVector &FlingDirection, float FlingForce);
	
	// Default force to be applied to flinging.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AppliedForce = 1000.f;
};
