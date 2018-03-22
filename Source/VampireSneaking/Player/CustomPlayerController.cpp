// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CustomPlayerController.h"
#include "Player/PlayableCharacterBase.h"
#include "Player/PlayerVamp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "HealthComponent.h"

ACustomPlayerController::ACustomPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	// Make health component.
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
}

void ACustomPlayerController::ChangePawn()
{
	// If the player is out of blood, they should'nt be able to change into batmode.
	if (!(HealthComponent && GetPawn() && Cast<APlayerVamp>(GetPawn())) || HealthComponent->IsOutOfBlood()) {
		return;
	}

	if (ControllablePawns.Num() != 0) {

		// Increment counter.
		CurrentIndex = (CurrentIndex + 1) % ControllablePawns.Num();

		if (ControllablePawns[CurrentIndex]) {
			MoveController(CurrentIndex);
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("There aren't any pawns assigned in the controllable pawns array!"));
	}
}

void ACustomPlayerController::ChangePawn(int index)
{
	// If the player is out of blood, they should'nt be able to change into batmode.
	if (!(HealthComponent && GetPawn() && Cast<APlayerVamp>(GetPawn())) || HealthComponent->IsOutOfBlood()) {
		return;
	}

	if (ControllablePawns.Num() != 0 && index >= 0 && index < ControllablePawns.Num() && ControllablePawns[index]) {
		if (GetPawn() && GetPawn() != ControllablePawns[index]) {
			MoveController(CurrentIndex);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Trying to switch to the same pawn!"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Index out of bounds!"));
	}

	CurrentIndex = index;
}

void ACustomPlayerController::MoveController(int index)
{
	APawn *currentlyPossessed = GetPawn();
	UnPossess();

	SwapActorLocation(Cast<AActor>(currentlyPossessed), Cast<AActor>(ControllablePawns[index]));

	Possess(ControllablePawns[index]);

	// Transfer stats
	ControllablePawns[index]->GetMovementComponent()->Velocity = currentlyPossessed->GetVelocity();
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("BatTransform", EInputEvent::IE_Pressed, this, &ACustomPlayerController::ChangePawn);
	InputComponent->BindAction("BatTransform", EInputEvent::IE_Released, this, &ACustomPlayerController::ChangePawn);

	InputComponent->BindAction("Bite", IE_Pressed, this, &ACustomPlayerController::ToggleSuckBlood);
	InputComponent->BindAction("Bite", IE_Released, this, &ACustomPlayerController::ToggleSuckBlood);
}

void ACustomPlayerController::SwapActorLocation(AActor * first, AActor * second)
{
	if (first && second) {
		FVector tempLocation{ first->GetActorLocation() };
		first->SetActorLocation(second->GetActorLocation());
		second->SetActorLocation(tempLocation);
	}
	else {
		return;
	}
}

void ACustomPlayerController::ToggleSuckBlood()
{
	PressingBloodSuckButton = !PressingBloodSuckButton;
}

const bool ACustomPlayerController::GetBloodSuckButton()
{
	return PressingBloodSuckButton;
}
