// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Player/DamageType_Explosion.h"
#include "Engine/TargetPoint.h"
#include "Player/PlayerVamp.h"
#include "AI/EnemyAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"
#include "AI/BTShootAtPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float temp = AActor::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	APlayerVamp *player = Cast<APlayerVamp>(DamageCauser);

	if (DamageEvent.DamageTypeClass == ExplosionDamage) {
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) {
			FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
			FVector FlingDirection = PointDamageEvent->ShotDirection;

			if (GetController()) {
				AEnemyAI *enemyAI = Cast<AEnemyAI>(GetController());
				
				FExplosionDamageInfo damageInfo{ DamageAmount, FlingDirection, player ? player->HitForce : 1.f };
				enemyAI->DamageInfo = damageInfo;

				if (enemyAI && enemyAI->GetBlackboardComponent()) {
					
					if (!enemyAI->GetBlackboardComponent()->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(AIState::Attacked))) {
						UE_LOG(LogTemp, Warning, TEXT("Failed to set enum state in blackboard."));
					}
				}
			}
		}
		else if (DamageEvent.IsOfType(FRadialDamageEvent::ClassID)) {
			FRadialDamageEvent* const RadialDamageEvent = (FRadialDamageEvent*)&DamageEvent;
			// UE_LOG(LogTemp, Warning, TEXT("Damaged with %f radial damage!"), DamageAmount + RadialDamageEvent->Params.GetDamageScale(FVector{ GetActorLocation() - RadialDamageEvent->Origin }.Size()));

			FVector FlingDirection = FVector{ GetActorLocation() - RadialDamageEvent->Origin };

			UDamageType_Explosion::HandleDamage(this, FlingDirection, RadialDamageEvent->Params.GetDamageScale(FlingDirection.Size()) * ((player) ? player->HitForce : 1000.f));
			/*if (player) {
				UDamageType_Explosion::HandleDamage(this, FlingDirection, RadialDamageEvent->Params.GetDamageScale(FlingDirection.Size()) * player->HitForce);
			}
			else {
				UDamageType_Explosion::HandleDamage(this, FlingDirection, RadialDamageEvent->Params.GetDamageScale(FlingDirection.Size()) * 1000.f);
			}*/
		}
		else if (DamageEvent.IsOfType(FDamageEvent::ClassID)) {

			UDamageType_Explosion::HandleDamage(this, FVector{ GetActorLocation() - DamageCauser->GetActorLocation() });
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Unknown damageEvent. (Fix this warning in AEnemy::TakeDamage)"));
		}
	}

	return temp;
}

void AEnemy::SpawnBullet(FRotator BulletOrientation, FVector SpawnPosition, float Damage, AActor* BulletOwner) const
{
	if (GetWorld() == nullptr)
		return;

	FTransform spawnTrans{ BulletOrientation, SpawnPosition.IsZero() ? GetActorLocation() : SpawnPosition, FVector{ 1.f } };

	auto *projectile = Instigator != nullptr
	? GetWorld()->SpawnActorDeferred<AEnemyBullet>(ProjectileClass, spawnTrans, BulletOwner)
	: GetWorld()->SpawnActorDeferred<AEnemyBullet>(ProjectileClass, spawnTrans);

	if (projectile == nullptr)
		return;

	projectile->Damage = Damage;
	projectile->FinishSpawning(spawnTrans);
}

void AEnemy::ShootAtPlayer()
{
	APawn *player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (player == nullptr)
		return;

	if (GetMesh() != nullptr)
	{
		const FRotator enemyToPlayerRot{ FVector{ (player->GetActorLocation() + player->GetVelocity() * 0.2) - GetMesh()->GetSocketLocation(TEXT("Gun")) }.Rotation() };
		SpawnBullet(enemyToPlayerRot, GetMesh()->GetSocketLocation(TEXT("Gun")), 30.f, this);
	}
	else
	{
		const FRotator enemyToPlayerRot{ FVector{ (player->GetActorLocation() + player->GetVelocity() * 0.2) - GetActorLocation() }.Rotation() };
		SpawnBullet(enemyToPlayerRot, GetActorLocation(), 30.f, this);
	}
}

