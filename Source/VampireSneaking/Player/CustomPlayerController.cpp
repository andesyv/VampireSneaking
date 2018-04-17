// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/CustomPlayerController.h"
#include "Player/PlayableCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "HealthComponent.h"
#include "Player/FollowCamera.h"
#include "Wall.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/EnemyAI.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "Enemy.h"

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
	ChangePawn(-1);
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
						UE_LOG(LogTemp, Error, TEXT("Failed to set bool ResetAttackCooldown in blackboard!"));
					}
				}
			}
		}
	}
}

void ACustomPlayerController::ChangePawn(int index)
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

	// Hinder enemies targeting the player to reset their attack time.
	CancelEnemyCooldownReset();

	// Toggle vision range of enemies.
	if (!ToggleVisionRanges())
	{
		UE_LOG(LogTemp, Warning, TEXT("Could'nt switch between enemy vision ranges!"));
	}
}

bool ACustomPlayerController::ToggleVisionRanges() const
{
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		AVampireSneakingGameModeBase *gamemode = Cast<AVampireSneakingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gamemode)
		{
			const TArray<AEnemy*> enemies{ gamemode->GetEnemyList() };
			for (auto item : enemies)
			{
				if (item->GetController())
				{
					AEnemyAI *enemyAI = Cast<AEnemyAI>(item->GetController());
					if (enemyAI)
					{
						if (enemyAI->ToggleVisionRange())
						{
							// Continue to skip the return in the loop.
							continue;
						}
					}
				}
				return false;
			}
			// This is clearly the best outcome.
			return true;
		}
	}
	return false;
}

APawn* ACustomPlayerController::MoveController_Implementation(int index)
{
	APawn *currentlyPossessed = GetPawn();
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

void ACustomPlayerController::SetInvisWalls()
{
	// Raycast from camera to player
	TArray<FHitResult> Hits{};
	TArray<AActor*> newWalls{};
	if (followCamera && followCamera->ViewBlockingTrace(Hits))
	{
		for (auto item : Hits)
		{
			if (item.Actor.Get() != nullptr)
			{
				auto *wall = Cast<AWall>(item.Actor);
				if (wall)
				{
					// If wall is not in the invisible walls array, change it to not be visible.
					newWalls.Add(wall);
					if (!InvisibleWalls.Contains(wall))
					{
						wall->SetMaterialVisible(false);
					}
				}
			}
		}
	}

	// If wall is not in tracehit array, make it visible again.
	for (auto item : InvisibleWalls)
	{
		if (!newWalls.Contains(item))
		{
			auto *wall = Cast<AWall>(item);
			if (wall)
			{
				wall->SetMaterialVisible(true);
			}
		}
	}

	// Set the invisible walls list to be the newly traced items:
	InvisibleWalls = newWalls;
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
