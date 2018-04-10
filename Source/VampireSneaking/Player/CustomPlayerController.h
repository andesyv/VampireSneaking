// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

// Forward declarations
class UHealthComponent;
class UCameraComponent;
class AFollowCamera;

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
	ACustomPlayerController();

	void Possess(APawn* aPawn) override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Shall only be changed in the ChangePawn function.
	uint32 CurrentIndex{0};

	TArray<APawn*> ControllablePawns{};

	// The camera that follows the player
	AFollowCamera* followCamera = nullptr;

	void ChangePawn();
	void ChangePawn(int index);

	void MoveController(int index);

	/** Allows the PlayerController to set up custom input bindings. */
	void SetupInputComponent() override;

	// Is the player sucking blood?
	bool PressingBloodSuckButton{false};

	// Check if the player can change
	bool ChangeValid() const;

	// Function called when dying.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Death();

	// Checks for walls to be turned invisible
	UFUNCTION(BlueprintCallable)
	void SetInvisWalls();

	// SetInvisWalls timer handle
	FTimerHandle SetInvisWallsHandle;

public:

	// Health and blood component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent = nullptr;

	// FollowCamera class
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFollowCamera> followCameraClass;

	void SwapActorLocation(AActor* first, AActor* second);

	// Toggle blood sucking.
	void ToggleSuckBlood();

	// Getter for blood-sucking button.
	bool GetBloodSuckButton() const;
};
