// Fill out your copyright notice in the Description page of Project Settings.

#include "AIVisionCheck.h"
#include "AIController.h"
#include "Enemy.h"
#include "EnemyAI.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UAIVisionCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();

	if (blackboard) {
		AIState state{ GetState(OwnerComp, DeltaSeconds, static_cast<AIState>(blackboard->GetValue<UBlackboardKeyType_Enum>(State.SelectedKeyName))) };

		// If the GetState function could'nt return a state, quit while you're ahead.
		if (state == AIState::NoState) {
			return;
		}

		// Set the memory of the last position the AI saw the player. (No need to check for nullptr's, as that was done in the GetState function.
		if (state == AIState::Combat && !(blackboard->SetValue<UBlackboardKeyType_Vector>(blackboard->GetKeyID(LastSeenPosition.SelectedKeyName), UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation()))) {
			UE_LOG(LogTemp, Error, TEXT("Failed to set vector in blackboard!"));
		}

		// Set the actor to follow to be the player if the AI can see it, and nullptr if otherwise.
		if (!(blackboard->SetValue<UBlackboardKeyType_Object>(blackboard->GetKeyID(TargetActor.SelectedKeyName), (state == AIState::Combat) ? Cast<AActor>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()) : nullptr))) {
			UE_LOG(LogTemp, Error, TEXT("Failed to set actor in blackboard!"));
		}



		// Set the blackboard enum "State" to be the state gotten from GetState. (Should never be AIState::NoState)
		if (!(blackboard->SetValue<UBlackboardKeyType_Enum>(blackboard->GetKeyID(State.SelectedKeyName), static_cast<UBlackboardKeyType_Enum::FDataType>(state)))) {
			UE_LOG(LogTemp, Error, TEXT("Failed to set enum in blackboard!"));
		}
	}
}

AIState UAIVisionCheck::GetState(UBehaviorTreeComponent & OwnerComp, float DeltaSeconds, AIState lastState) {

	if (GetWorld() && !OwnerComp.GetAIOwner()) {
		return AIState::NoState;
	}

	AEnemy *enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	APlayerController *playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// Checking that no pointers are nullptr's.
	if (!playerController || !playerController->GetPawn() || !enemy) {
		return AIState::NoState;
	}

	// Setup additional stuff.
	FVector enemyToPlayer{ playerController->GetPawn()->GetActorLocation() - enemy->GetActorLocation() };
	FHitResult traceResult{};
	const FName TraceTag("VisionTrace");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams collisionQueryParams(TraceTag, false);
	static float timer{ 0.f };

	// Do the checks.
	if (FMath::Abs(GetAngleBetween(enemyToPlayer, enemy->GetActorForwardVector())) < enemy->VisionAngle	// Is player inside vision angle?
		&& enemyToPlayer.Size() < enemy->VisionRadius	// Is player inside vision radius?
		&& !(GetWorld()->LineTraceSingleByChannel(traceResult, enemy->GetActorLocation(), playerController->GetPawn()->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel3, collisionQueryParams)))	// Is there anything blocking the line of sight?
	{
		// The enemy can see the player.

		return AIState::Combat;
	}
	else if (lastState == AIState::Combat) {
		timer = 0.f;
		return AIState::Searching;
	}
	else if (lastState == AIState::Searching && timer < SearchTime) {
		timer += DeltaSeconds;
		return AIState::Searching;
	}
	else {
		return AIState::Idle;
	}
}

float UAIVisionCheck::GetAngleBetween(FVector pos1, FVector pos2)
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(pos1, pos2) / (pos1.Size() * pos2.Size())));
}
