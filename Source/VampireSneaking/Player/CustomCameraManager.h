// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "CustomCameraManager.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API ACustomCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
	
protected:
	/**
	* Calculates an updated POV for the given viewtarget.
	* @param	OutVT		ViewTarget to update.
	* @param	DeltaTime	Delta Time since last camera update (in seconds).
	*/
	// virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
	
	
	
};
