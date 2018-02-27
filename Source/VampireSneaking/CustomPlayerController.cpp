// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerController.h"
#include "PlayableCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void ACustomPlayerController::ChangePawn()
{
	if (ControllablePawns.Num() != 0) {

		// Increment counter.
		CurrentIndex = (CurrentIndex + 1) % ControllablePawns.Num();

		if (ControllablePawns[CurrentIndex]) {

			APawn *currentlyPossessed = GetPawn();
			UnPossess();

			SwapActorLocation(Cast<AActor>(currentlyPossessed), Cast<AActor>(ControllablePawns[CurrentIndex]));
			
			Possess(ControllablePawns[CurrentIndex]);

			// Transfer stats
			if (!TransferStats(ControllablePawns[CurrentIndex], currentlyPossessed)) {
				UE_LOG(LogTemp, Error, TEXT("Failed to transfer stats when switching pawns!"));
			}
		}

		return;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("There aren't any pawns assigned in the controllable pawns array!"));
	}
}

bool ACustomPlayerController::TransferStats(APawn * newPawn, APawn * oldPawn)
{
	APlayableCharacterBase *newCharacterBase = Cast<APlayableCharacterBase>(newPawn);
	APlayableCharacterBase *oldCharacterBase = Cast<APlayableCharacterBase>(oldPawn);
	if (newCharacterBase && oldCharacterBase) {
		newCharacterBase->Health = oldCharacterBase->Health;
		newCharacterBase->Blood = oldCharacterBase->Blood;
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
