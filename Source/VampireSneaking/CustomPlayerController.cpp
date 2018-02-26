// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerController.h"

bool ACustomPlayerController::ChangePawn()
{
	if (ControllablePawns.Num() != 0) {

		// Increment counter.
		CurrentIndex = (CurrentIndex + 1) % ControllablePawns.Num();

		if (ControllablePawns[CurrentIndex]) {

			APawn *currentlyPossessed = GetPawn();
			UnPossess();

			SwapActorLocation(Cast<AActor>(currentlyPossessed), Cast<AActor>(ControllablePawns[CurrentIndex]));
			
			Possess(ControllablePawns[CurrentIndex]);
		}

		return true;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("There aren't any pawns assigned in the controllable pawns array!"));
		return false;
	}
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
