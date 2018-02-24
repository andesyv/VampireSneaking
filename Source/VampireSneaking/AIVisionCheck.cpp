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
		if (GetWorld() && !OwnerComp.GetAIOwner()) {
			return;
		}

		AEnemy *enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
		APlayerController *playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		// Checking that no pointers are nullptr's.
		if (!playerController || !playerController->GetPawn() || !enemy) {
			return;
		}

		// Setup additional stuff.
		AIState state{AIState::Idle};
		FVector enemyToPlayer{ playerController->GetPawn()->GetActorLocation() - enemy->GetActorLocation() };
		FHitResult traceResult{};
		const FName TraceTag("VisionTrace");
		// GetWorld()->DebugDrawTraceTag = TraceTag;
		FCollisionQueryParams collisionQueryParams(TraceTag, false);

		// Do the checks.
		if (FMath::Abs(GetAngleBetween(enemyToPlayer, enemy->GetActorForwardVector())) < enemy->VisionAngle	// Is player inside vision angle?
			&& enemyToPlayer.Size() < enemy->VisionRadius	// Is player inside vision radius?
			&& !(GetWorld()->LineTraceSingleByChannel(traceResult, enemy->GetActorLocation(), playerController->GetPawn()->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel3, collisionQueryParams)))	// Is there anything blocking the line of sight?
		{
			// The enemy can see the player.

			state = AIState::Combat;
		}
		else {
			state = AIState::Idle;
		}		

		if (!(blackboard->SetValue<UBlackboardKeyType_Enum>(blackboard->GetKeyID(State.SelectedKeyName), static_cast<UBlackboardKeyType_Enum::FDataType>(state)))) {
			UE_LOG(LogTemp, Error, TEXT("Failed to set enum in blackboard!"));
		}
	}
}

float UAIVisionCheck::GetAngleBetween(FVector pos1, FVector pos2)
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(pos1, pos2) / (pos1.Size() * pos2.Size())));
}
