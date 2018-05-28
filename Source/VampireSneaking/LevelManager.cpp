// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelManager.h"
#include "Kismet/GameplayStatics.h"

ULevelManager::ULevelManager()
{
}

int ULevelManager::NextLevel()
{
	if ((CurrentLevel + 1) < LevelList.Num())
	{
		CurrentLevel++;
		UGameplayStatics::OpenLevel(this, LevelList[CurrentLevel]);
	}
	return CurrentLevel;
}

int ULevelManager::PreviousLevel()
{
	if (CurrentLevel - 1 >= 0)
	{
		CurrentLevel--;
		UGameplayStatics::OpenLevel(this, LevelList[CurrentLevel]);
	}
	return CurrentLevel;
}
