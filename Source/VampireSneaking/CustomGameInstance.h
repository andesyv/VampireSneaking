// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LevelManager.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	/** virtual function to allow custom GameInstances an opportunity to set up what it needs */
	void Init() override;

	/** Call to create the game mode for a given map URL */
	class AGameModeBase* CreateGameModeForURL(FURL InURL) override;

	// Switches to the next level and returns new level index.
	UFUNCTION(BlueprintCallable)
	int NextLevel();

	const ULevelManager &GetLevelManager() const { return *LevelManager; };
	
protected:

	// The level manager class.
	ULevelManager *LevelManager;
};
