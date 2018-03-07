// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CustomPlayerController.h"
#include "Player/PlayableCharacterBase.h"
#include "Player/PlayerVamp.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACustomPlayerController::ChangePawn()
{
	// If the player is out of blood, they should'nt be able to change into batmode.
	if (CheckIfBloodIsMissing() && GetPawn() && Cast<APlayerVamp>(GetPawn())) {
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
	if (CheckIfBloodIsMissing() && GetPawn() && Cast<APlayerVamp>(GetPawn())) {
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
	if (!TransferStats(ControllablePawns[index], currentlyPossessed)) {
		UE_LOG(LogTemp, Error, TEXT("Failed to transfer stats when switching pawns!"));
	}
}

bool ACustomPlayerController::CheckIfBloodIsMissing()
{
	if (GetPawn()) {
		APlayableCharacterBase *charBase = Cast<APlayableCharacterBase>(GetPawn());
		return charBase->IsOutOfBlood();
	}
	return false;
}

bool ACustomPlayerController::TransferStats(APawn * newPawn, APawn * oldPawn)
{
	APlayableCharacterBase *newCharacterBase = Cast<APlayableCharacterBase>(newPawn);
	APlayableCharacterBase *oldCharacterBase = Cast<APlayableCharacterBase>(oldPawn);
	if (newCharacterBase && oldCharacterBase) {
		newCharacterBase->Health = oldCharacterBase->Health;
		newCharacterBase->Blood = oldCharacterBase->Blood;
		newCharacterBase->OutOfBlood = oldCharacterBase->OutOfBlood;
		newCharacterBase->SuckingBlood = oldCharacterBase->SuckingBlood;
		newCharacterBase->GetMovementComponent()->Velocity = oldCharacterBase->GetVelocity();

		return true;
	}
	else {
		return false;
	}
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("BatTransform", EInputEvent::IE_Pressed, this, &ACustomPlayerController::ChangePawn);
	InputComponent->BindAction("BatTransform", EInputEvent::IE_Released, this, &ACustomPlayerController::ChangePawn);
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
