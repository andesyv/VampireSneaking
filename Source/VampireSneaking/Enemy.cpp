// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Player/DamageType_Explosion.h"
#include "Kismet/GameplayStatics.h"
#include "VampireSneakingGameModeBase.h"
#include "Player/PlayerVamp.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	/*
	if (UGameplayStatics::GetGameMode(GetWorld())) {
		AVampireSneakingGameModeBase *gameMode = Cast<AVampireSneakingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gameMode) {
			gameMode->GetEnemyList().Add(this);
		}
	}
	*/
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float temp = AActor::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	APlayerVamp *player = Cast<APlayerVamp>(DamageCauser);

	if (DamageEvent.DamageTypeClass == ExplosionDamage) {
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID)) {
			FPointDamageEvent* const PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
			FVector FlingDirection = PointDamageEvent->ShotDirection;

			if (player) {
				UDamageType_Explosion::HandleDamage(this, FlingDirection, player->HitForce);
			}
			else {
				UDamageType_Explosion::HandleDamage(this, FlingDirection);
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
			UE_LOG(LogTemp, Warning, TEXT("I guess it was no damage?"));
		}
	}

	return temp;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

