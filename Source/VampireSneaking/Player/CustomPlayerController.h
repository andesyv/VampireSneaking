// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

// Forward declarations
class UHealthComponent;

/**
 * A custom playercontroller.
 */
UCLASS()
class VAMPIRESNEAKING_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

	friend class AVampireSneakingGameModeBase;
	friend class ABatMode;

public:
	/**
	 * Default constructor for ACustomPlayerController
	 */
	ACustomPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Shall only be changed in the ChangePawn function.
	uint32 CurrentIndex{ 0 };

	TArray<APawn*> ControllablePawns{};

	void ChangePawn();
	void ChangePawn(int index);

	void MoveController(int index);

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

	// Is the player sucking blood?
	bool PressingBloodSuckButton{ false };	

	// Check if the player can change
	const bool ChangeValid() const;
	
public:

	// Health and blood component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthComponent *HealthComponent = nullptr;

	void SwapActorLocation(AActor *first, AActor *second);

	// Toggle blood sucking.
	void ToggleSuckBlood();

	// Getter for blood-sucking button.
	const bool GetBloodSuckButton();
};
