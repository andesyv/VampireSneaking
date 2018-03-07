// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTShootAtPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Player/PlayableCharacterBase.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"

EBTNodeResult::Type UBTShootAtPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	timer = 0;
	bNotifyTick = 1;
	return EBTNodeResult::InProgress;
}

void UBTShootAtPlayer::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (timer > ShootTime) {
		Shoot(&OwnerComp);
	}
	timer += DeltaSeconds;
}

void UBTShootAtPlayer::Shoot_Implementation(UBehaviorTreeComponent *OwnerComp)
{
	UBlackboardComponent *blackboard = OwnerComp->GetBlackboardComponent();
	if (blackboard) {
		APlayableCharacterBase *player = Cast<APlayableCharacterBase>(blackboard->GetValue<UBlackboardKeyType_Object>(TargetActor.SelectedKeyName));
		if (player) {
			player->TakeDamage(Damage);
			timer = 0.f;
			if (OwnerComp->GetAIOwner() && OwnerComp->GetAIOwner()->GetPawn()) {
				PlayExplotion(OwnerComp->GetAIOwner()->GetPawn(), player);
			}
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Can't receive actor from blackboard!"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't receive blackboard!"));
	}
	FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
	return;
}

void UBTShootAtPlayer::PlayExplotion(AActor *enemy, AActor *player)
{
	if (ExplotionFX) {
		APlayableCharacterBase *playerChar = Cast<APlayableCharacterBase>(player);
		if (playerChar) {
			UCapsuleComponent *playerCollider = Cast<UCapsuleComponent>(playerChar->GetRootComponent());
			if (playerCollider) {
				UE_LOG(LogTemp, Warning, TEXT("SHOT'S FIRED!"));
				FVector playerToEnemy{ player->GetActorLocation() - enemy->GetActorLocation() };
				playerToEnemy.Normalize();
				FTransform trans{ FRotator::ZeroRotator, player->GetActorLocation() - playerToEnemy * playerCollider->GetScaledCapsuleRadius(), FVector{ 1.f, 1.f, 1.f } };
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplotionFX, trans, true);
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Unable to get player collider."));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Unable to get player."));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("ExplotionFX is not set on task: \"Shoot at player\" in behavior tree."));
	}
}
