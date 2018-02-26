// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVamp.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerVamp::APlayerVamp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set health.
	Health = GetMaxHealth();
}

// Called when the game starts or when spawned
void APlayerVamp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerVamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerVamp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

const float APlayerVamp::GetHealth() const
{
	return Health;
}

const float APlayerVamp::GetMaxHealth() const
{
	return MaxHealth;
}

const float APlayerVamp::GetPercentageHealth() const
{
	return Health / MaxHealth;
}

const float APlayerVamp::TakeDamage(float damage)
{
	Health -= damage;
	if (Health <= 0) {
		Health = 0;
		ded = true;
	}
	return Health;
}
