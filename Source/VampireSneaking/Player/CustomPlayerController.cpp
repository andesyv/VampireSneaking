// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CustomPlayerController.h"
#include "Player/PlayableCharacterBase.h"
#include "Player/PlayerVamp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "HealthComponent.h"
#include "Player/FollowCamera.h"
#include "Wall.h"
#include "TimerManager.h"

ACustomPlayerController::ACustomPlayerController() {
	// Make health component.
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));

	// Add death event.
	if (HealthComponent) {
		HealthComponent->OnDeath.AddDynamic(this, &ACustomPlayerController::Death);
	}
}

void ACustomPlayerController::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);

	if (followCamera != nullptr)
	{
		followCamera->Target = aPawn;
	}
}

void ACustomPlayerController::BeginPlay()
{
	if (GetWorld())
	{
		followCamera = GetWorld()->SpawnActor<AFollowCamera>(followCameraClass, (GetPawn() != nullptr) ? GetPawn()->GetActorLocation() : FVector{ 0.f, 0.f, 0.f }, FRotator::ZeroRotator);
		if (followCamera != nullptr && GetPawn())
		{
			followCamera->Target = GetPawn();
			SetViewTarget(followCamera);
		}
	}
	GetWorldTimerManager().SetTimer(SetInvisWallsHandle, this, &ACustomPlayerController::SetInvisWalls, 0.1f, true);
}

void ACustomPlayerController::ChangePawn()
{
	// If the player is out of blood, they should'nt be able to change into batmode.
	if (!ChangeValid()) {
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
	if (!ChangeValid()) {
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
}

void ACustomPlayerController::ToggleSuckBlood()
{
	PressingBloodSuckButton = !PressingBloodSuckButton;
}

bool ACustomPlayerController::GetBloodSuckButton() const
{
	return PressingBloodSuckButton;
}

bool ACustomPlayerController::ChangeValid() const
{
	return HealthComponent && GetPawn() && !(HealthComponent->IsOutOfBlood() && GetPawn()->IsA(APlayerVamp::StaticClass()));
}

void ACustomPlayerController::SetInvisWalls()
{
	// Raycast from camera to player
	TArray<FHitResult> Hits{};
	if (followCamera && followCamera->ViewBlockingTrace(Hits))
	{
		for (auto item : Hits)
		{
			if (item.Actor.Get() != nullptr)
			{
				auto *wall = Cast<AWall>(item.Actor);
				if (wall)
				{
					wall->SetMaterialVisible(false);
					UE_LOG(LogTemp, Warning, TEXT("Found %s"), *wall->GetFName().ToString())
				}
			}
		}
	}

	// Get list of view-blocking objects

	// Set material property of view-blocking objects
}

void ACustomPlayerController::Death_Implementation() {
	// Call death event.
	if (GetWorld() && GetWorld()->GetAuthGameMode<AVampireSneakingGameModeBase>()) {
		GetWorld()->GetAuthGameMode<AVampireSneakingGameModeBase>()->PlayerDies();
	}

	// Destory all pawns.
	UnPossess();
	for (auto pawn : ControllablePawns) {
		pawn->Destroy();
	}

	// Destroy yoself!
	Destroy();
}
