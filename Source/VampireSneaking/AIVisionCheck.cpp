// Fill out your copyright notice in the Description page of Project Settings.

#include "AIVisionCheck.h"
#include "AIController.h"
#include "Enemy.h"
#include "EnemyAI.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UAIVisionCheck::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();

	if (blackboard) {
		if (!OwnerComp.GetAIOwner()) {
			return;
		}

		AEnemy *enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
		APlayerController *playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		// Checking that no pointers are nullptr's.
		if (!playerController || !playerController->GetPawn() || !enemy) {
			return;
		}

		if (FMath::Abs(GetAngleBetween(
			FVector{ playerController->GetPawn()->GetActorLocation() - enemy->GetActorLocation() }
			, enemy->GetActorForwardVector())) < VisionAngle) {
			// Player is inside vision angle.

			if (!(blackboard->SetValue<UBlackboardKeyType_Enum>(blackboard->GetKeyID(State.SelectedKeyName), static_cast<UBlackboardKeyType_Enum::FDataType>(AIState::Combat)))) {
				UE_LOG(LogTemp, Error, TEXT("Failed to set enum in blackboard!"));
			}
		}
		else {
			if (!(blackboard->SetValue<UBlackboardKeyType_Enum>(blackboard->GetKeyID(State.SelectedKeyName), static_cast<UBlackboardKeyType_Enum::FDataType>(AIState::Idle)))) {
				UE_LOG(LogTemp, Error, TEXT("Failed to set enum in blackboard!"));
			}
		}
	}
}

float UAIVisionCheck::GetAngleBetween(FVector pos1, FVector pos2)
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(pos1, pos2) / (pos1.Size() * pos2.Size())));
}
