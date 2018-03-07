// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class VAMPIRESNEAKING_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

	friend class AVampireSneakingGameModeBase;
	friend class ABatMode;

protected:
	// Shall only be changed in the ChangePawn function.
	uint32 CurrentIndex{ 0 };

	TArray<APawn*> ControllablePawns{};

	void ChangePawn();
	void ChangePawn(int index);

	void MoveController(int index);
	bool CheckIfBloodIsMissing();

	bool TransferStats(APawn *newPawn, APawn *oldPawn);

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;
	
public:

	void SwapActorLocation(AActor *first, AActor *second);
};
