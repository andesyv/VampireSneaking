// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LevelManager.generated.h"

/**
 * A custom level manager
 * Selects the right level.
 */
UCLASS()
class VAMPIRESNEAKING_API ULevelManager : public UObject
{
	GENERATED_BODY()

protected:
	const TArray<FName> LevelList
	{
		FName{"AlfaMap"},
		FName{"TransitionMap"},
		FName{"BetaMap"},
	};

	int CurrentLevel{ 0 };

public:
	ULevelManager();

	int NextLevel();
	int PreviousLevel();
};
