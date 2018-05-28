// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomGameInstance.h"
#include "VampireSneakingGameModeBase.h"

void UCustomGameInstance::Init()
{
	Super::Init();

	LevelManager = CreateDefaultSubobject<ULevelManager>(TEXT("LevelManager"));
	
	// This is crash worthy.
	check(LevelManager != nullptr);
}

/** Call to create the game mode for a given map URL */
class AGameModeBase* UCustomGameInstance::CreateGameModeForURL(FURL InURL)
{
	AGameModeBase *gamemode = Super::CreateGameModeForURL(InURL);

	if (gamemode)
	{
		AVampireSneakingGameModeBase *vampireGamemode = Cast<AVampireSneakingGameModeBase>(gamemode);
		if (vampireGamemode != nullptr)
		{
			vampireGamemode->GameInstance = this;
		}
	}

	return gamemode;
}

int UCustomGameInstance::NextLevel()
{
	return LevelManager->NextLevel();
}

