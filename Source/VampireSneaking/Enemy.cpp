// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Player/DamageType_Explosion.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float temp = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	TSubclassOf<UDamageType> damageType = DamageEvent.DamageTypeClass;
	if (DamageEvent.DamageTypeClass == UDamageType_Explosion::StaticClass()) {
		UDamageType_Explosion::HandleDamage();
	}
	UE_LOG(LogTemp, Warning, TEXT("Damaged with %d damage!"), Damage);

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

