// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTShootAtPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Player/PlayableCharacterBase.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Player/CustomPlayerController.h"
#include "HealthComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

EBTNodeResult::Type UBTShootAtPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{ 
	UBlackboardComponent *blackboard = OwnerComp.GetBlackboardComponent();
	if (blackboard && blackboard->GetValue<UBlackboardKeyType_Bool>(ResetAttackCooldown.SelectedKeyName))
	{
		timer = 0;
		if (!blackboard->SetValue<UBlackboardKeyType_Bool>(ResetAttackCooldown.SelectedKeyName, true))
		{
			UE_LOG(LogTemp, Warning, TEXT("Unable to set bool blackboard parameter."));
		}
	}
	bNotifyTick = 1;
	return EBTNodeResult::InProgress;
}

void UBTShootAtPlayer::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	APawn *player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APawn *enemy = OwnerComp.GetAIOwner()->GetPawn();
	if (player && enemy) {
		FVector toPlayer{ player->GetActorLocation() - enemy->GetActorLocation() };
		toPlayer.Z = 0;
		enemy->SetActorRotation(toPlayer.Rotation());
	}

	if (timer > ShootTime) {
		// Shoot(&OwnerComp);
	}	
	timer += DeltaSeconds;
}

void UBTShootAtPlayer::Shoot_Implementation(UBehaviorTreeComponent *OwnerComp)
{
	if (OwnerComp == nullptr)
	{
		return;
	}

	UBlackboardComponent *blackboard{ const_cast<UBlackboardComponent*>(OwnerComp->GetBlackboardComponent()) };
	if (blackboard) {
		APlayableCharacterBase *player = Cast<APlayableCharacterBase>(blackboard->GetValue<UBlackboardKeyType_Object>(TargetActor.SelectedKeyName));
		/*if (player && player->GetController()) {
			ACustomPlayerController *playerController = Cast<ACustomPlayerController>(player->GetController());
			if (playerController && playerController->HealthComponent) {
				playerController->HealthComponent->TakeDamage(Damage);
				timer = 0.f;
				if (OwnerComp->GetAIOwner() && OwnerComp->GetAIOwner()->GetPawn()) {
					PlayExplotion(OwnerComp->GetAIOwner()->GetPawn(), player);
				}
				FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
				return;
			}
		}*/
		if (player && OwnerComp->GetAIOwner()->GetPawn())
		{
			FVector enemyToPlayer{ (player->GetActorLocation() + player->GetVelocity() * 0.2) - OwnerComp->GetAIOwner()->GetPawn()->GetActorLocation() };
			timer = 0.f;

			SpawnBullet(OwnerComp, enemyToPlayer.Rotation(), OwnerComp->GetAIOwner()->GetPawn()->GetActorLocation());

			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
			return;
		}

		else {
			UE_LOG(LogTemp, Error, TEXT("Can't receive actor from blackboard! - In BTShootAyPlayer.cpp"));
		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Can't receive blackboard! - In BTShootAyPlayer.cpp"));
	}
	FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
}

void UBTShootAtPlayer::SpawnBullet(UBehaviorTreeComponent* OwnerComp, FRotator BulletOrientation, FVector SpawnPosition) const
{
	if (!(OwnerComp && OwnerComp->GetAIOwner() && GetWorld()))
	{
		return;
	}
	FTransform spawnTrans{ BulletOrientation, SpawnPosition, FVector{1.f} };
	auto *projectile = GetWorld()->SpawnActorDeferred<AEnemyBullet>(ProjectileClass, spawnTrans, OwnerComp->GetAIOwner());
	projectile->Damage = Damage;
	projectile->FinishSpawning(spawnTrans);
}

void UBTShootAtPlayer::PlayExplotion(AActor *enemy, AActor *player)
{
	if (ExplotionFX) {
		APlayableCharacterBase *playerChar = Cast<APlayableCharacterBase>(player);
		if (playerChar) {
			UCapsuleComponent *playerCollider = Cast<UCapsuleComponent>(playerChar->GetRootComponent());
			if (playerCollider) {
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