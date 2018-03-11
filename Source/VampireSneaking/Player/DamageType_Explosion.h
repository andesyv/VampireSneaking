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

		// UDamageType_Explosion();

public:
	// Handle the damage.
	static void HandleDamage(APawn *HitPawn, const FVector &FlingDirection = FVector{ 0.f, 1.f, 0.f }, float FlingForce = 1.f);
	
};
