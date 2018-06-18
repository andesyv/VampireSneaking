// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CustomPlayerController.h"
#include "Player/PlayableCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "HealthComponent.h"
#include "Player/FollowCamera.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Enemy.h"
#include "WallFadeComponent.h"

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
		GetWorld()->GetTimerManager().SetTimer(SetInvisWallsHandle, this, &ACustomPlayerController::SetInvisWalls, 0.1f, true);
	}
}

void ACustomPlayerController::QuitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}

void ACustomPlayerController::ChangePawn()
{
	ChangePawn(-1);
}

void ACustomPlayerController::SetParticles(APawn* CurrentPawn) const
{
	if (GetPawn() && GetWorld() && followCamera)
	{
		TArray<USpringArmComponent*> Comps{};
		followCamera->GetComponents<USpringArmComponent>(Comps);
		if (Comps.Num() >= 1)
		{
			// FTransform spawnTrans{ FRotator::ZeroRotator, GetPawn()->GetActorLocation() + FVector{ Comps[0]->GetUnfixedCameraPosition() - followCamera->GetActorLocation() } *0.2f };
			/*UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TransformEffect, spawnTrans, true);*/
			UParticleSystemComponent *particleSystem = UGameplayStatics::SpawnEmitterAttached(TransformEffect, followCamera->GetRootComponent(), NAME_None, FVector{ Comps[0]->GetUnfixedCameraPosition() - followCamera->GetActorLocation() } *0.2f);
			FVector particleVelocity{ CurrentPawn->GetVelocity().GetSafeNormal()};
			particleSystem->SetVectorRandParameter(TEXT("VelocityModifier"), particleVelocity.RotateAngleAxis(20.f, FVector{ 0.f, 0.f, 1.f }), particleVelocity.RotateAngleAxis(-20.f, FVector{ 0.f, 0.f, 1.f }));
			particleSystem->SetVectorParameter(TEXT("SmokeVelocity"), CurrentPawn->GetVelocity());
			// UE_LOG(LogTemp, Warning, TEXT("Velocity is %s"), *particleVelocity.ToString())
		}
	}
}

void ACustomPlayerController::ChangePawn(const int index)
{
	APawn* CurrentPawn{ nullptr };
	if (ControllablePawns.Num() != 0) {
		if (index >= 0 && index < ControllablePawns.Num() && ControllablePawns[index]) {

			if (GetPawn() && GetPawn() != ControllablePawns[index]) {
				CurrentPawn = MoveController(CurrentIndex);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Trying to switch to the same pawn!"));
				return;
			}

			CurrentIndex = index;
		} else {

			// Increment counter.
			CurrentIndex = (CurrentIndex + 1) % ControllablePawns.Num();

			if (ControllablePawns[CurrentIndex]) {
				CurrentPawn = MoveController(CurrentIndex);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There aren't any pawns assigned in the controllable pawns array!"));
	}

	// Set switching particles.
	SetParticles(CurrentPawn);

	// Hinder enemies targeting the player to reset their attack time.
	CancelEnemyCooldownReset();

	
}

void ACustomPlayerController::CancelEnemyCooldownReset()
{
	// Make a copy of the array in case it changes in runtime. (That has to be some fast switching if that's the case..)
	const TArray<AEnemyAI*> enemiesTargetingCopy{ EnemiesTargeting };
	for (auto item : enemiesTargetingCopy)
	{
		if (item && item->GetBrainComponent())
		{
			UBehaviorTreeComponent *behaviorTree = Cast<UBehaviorTreeComponent>(item->GetBrainComponent());
			if (behaviorTree)
			{
				UBlackboardComponent *enemyBlackboard = behaviorTree->GetBlackboardComponent();
				if (enemyBlackboard)
				{
					if (!enemyBlackboard->SetValue<UBlackboardKeyType_Bool>(TEXT("ResetAttackCooldown"), false))
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to set bool ResetAttackCooldown in blackboard on %s!"), *item->GetFName().ToString());
					}
				}
			}
		}
	}
}

APawn* ACustomPlayerController::MoveController_Implementation(int index)
{
	APawn *currentlyPossessed = GetPawn();
	if (currentlyPossessed == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Controller isn't possessing anything!"));
		return nullptr;
	}
	UnPossess();

	SwapActorLocation(Cast<AActor>(currentlyPossessed), Cast<AActor>(ControllablePawns[index]));

	Possess(ControllablePawns[index]);

	// Transfer stats
	ControllablePawns[index]->GetMovementComponent()->Velocity = currentlyPossessed->GetVelocity();

	return ControllablePawns[index];
}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// InputComponent->BindAction("BatTransform", EInputEvent::IE_Pressed, this, &ACustomPlayerController::ChangePawn);
	// InputComponent->BindAction("BatTransform", EInputEvent::IE_Released, this, &ACustomPlayerController::ChangePawn);

	InputComponent->BindAction("Bite", IE_Pressed, this, &ACustomPlayerController::ToggleSuckBlood);
	InputComponent->BindAction("Bite", IE_Released, this, &ACustomPlayerController::ToggleSuckBlood);
	InputComponent->BindAction("Quit", IE_Pressed, this, &ACustomPlayerController::QuitGame);
}

const APawn * ACustomPlayerController::GetControllablePawn(int index) const
{
	if (ControllablePawns.Num() > 0 && index >= 0 && index < ControllablePawns.Num() && ControllablePawns[index])
	{
		return ControllablePawns[index];
	}

	return nullptr;
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

void ACustomPlayerController::SetInvisWalls()
{
	// Raycast from camera to player
	TArray<FHitResult> Hits{};
	TArray<UWallFadeComponent*> newWalls{};
	if (followCamera && followCamera->ViewBlockingTrace(Hits))
	{
		for (auto item : Hits)
		{
			if (item.Actor.Get() != nullptr)
			{
				TArray<UWallFadeComponent*> WallComponents;
				item.Actor->GetComponents<UWallFadeComponent>(WallComponents);
				for (auto item : WallComponents)
				{
					if (item)
					{
						// If wall is not in the invisible walls array, change it to not be visible.
						newWalls.Add(item);
						if (!InvisibleWalls.Contains(item))
						{
							item->SetMaterialVisible(false);
						}
						break;
					}
				}
			}
		}
	}

	// If wall is not in tracehit array, make it visible again.
	for (auto item : InvisibleWalls)
	{
		if (item && !newWalls.Contains(item))
		{
			item->SetMaterialVisible(true);
		}
	}

	// Set the invisible walls list to be the newly traced items:
	InvisibleWalls = newWalls;
}

void ACustomPlayerController::Death_Implementation() {
	APawn *pawn = GetPawn();
	if (pawn)
	{
		APlayableCharacterBase *player = Cast<APlayableCharacterBase>(pawn);
		if (player)
		{
			player->dead = true;
		}

		// Call death event.
		if (GetWorld()) {
			AVampireSneakingGameModeBase *gamemode = GetWorld()->GetAuthGameMode<AVampireSneakingGameModeBase>();
			if (gamemode)
			{
				gamemode->PlayerDeath(this);
				gamemode->pawnToBeDestroyed = pawn;
			}
		}
	}
	UnPossess();

	

	// Destroy yoself!
	// Destroy();
}
