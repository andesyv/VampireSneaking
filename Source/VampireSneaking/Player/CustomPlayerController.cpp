// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CustomPlayerController.h"
#include "Player/PlayableCharacterBase.h"
#include "Player/PlayerVamp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/FollowCamera.h"

ACustomPlayerController::ACustomPlayerController() {
	// Make health component.
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));

	// Add death event.
	if (HealthComponent) {
		HealthComponent->OnDeath.AddDynamic(this, &ACustomPlayerController::Death);
	}

	// Make a dummy root-component.
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	// Make camera
	MainViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainView"));
	if (MainViewCamera)
	{
		MainViewCamera->SetupAttachment(RootComponent);
		MainViewCamera->SetRelativeLocation(FVector{ 0.f, 100.f, -100.f });
		MainViewCamera->SetRelativeRotation(FRotator{70.f, 0.f, 0.f});
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("Camera failed to create itself!"));
	}
}

void ACustomPlayerController::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);

	if (followCamera)
	{
		// followCamera->Target = aPawn;
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("There's no follow camera?!"));
	}
}

void ACustomPlayerController::BeginPlay()
{
	if (GetWorld())
	{
		followCamera = GetWorld()->SpawnActor<AFollowCamera>(followCameraClass, (GetPawn() != nullptr) ? GetPawn()->GetActorLocation() : FVector{ 0.f, 0.f, 0.f }, FRotator::ZeroRotator);
		// Why do this when you can just edit it in the follow camera blueprint?
		/*
		if (followCamera)
		{
			followCamera->SetTranslationAndRotation(MainViewCamera->RelativeLocation, MainViewCamera->RelativeRotation);
		}
		*/
	}
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

const bool ACustomPlayerController::ChangeValid() const
{
	return HealthComponent && GetPawn() && !(HealthComponent->IsOutOfBlood() && GetPawn()->IsA(APlayerVamp::StaticClass()));
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

const UCameraComponent* ACustomPlayerController::GetViewCamera() const {
	// return (MainViewCamera != nullptr) ? MainViewCamera : nullptr;
	if (followCamera)
	{
		return followCamera->Camera;
	}
	return MainViewCamera;
}
