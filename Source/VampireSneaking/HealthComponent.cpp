// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "Player/PlayableCharacterBase.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set health.
	Health = GetMaxHealth();
}

/*
// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
*/

const float UHealthComponent::GetHealth() const
{
	return Health;
}

const float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

const float UHealthComponent::GetPercentageHealth() const
{
	if (Health < 0.f) {
		return 0.f;
	}
	return Health / MaxHealth;
}

const float UHealthComponent::TakeDamage(float damage)
{
	Health -= damage;
	if (Health <= 0) {
		Health = 0;
		ded = true;

		if (GetOwner()) {	
		
			// Call death event (if player).
			if (GetOwner()->IsA(APlayableCharacterBase::StaticClass()) && Cast<AVampireSneakingGameModeBase>(GetWorld()->GetAuthGameMode())) {
				Cast<AVampireSneakingGameModeBase>(GetWorld()->GetAuthGameMode())->PlayerDies();
			}
			
			// Destroy actor.
			GetOwner()->Destroy();
		}
	}
	return Health;
}

const float UHealthComponent::GetBlood() const
{
	return Blood;
}

const float UHealthComponent::GetMaxBlood() const
{
	return MaxBlood;
}

const float UHealthComponent::GetPercentageBlood() const
{
	if (Blood < 0.f) {
		return 0.f;
	}
	return Blood / MaxBlood;
}

const float UHealthComponent::AddBlood(float amount)
{
	Blood += amount;
	if (Blood < 0.f) {
		Blood = 0.f;
		OutOfBlood = true;
	}
	else if (Blood > 0.f) {
		OutOfBlood = false;
	}

	return Blood;
}

const bool UHealthComponent::IsOutOfBlood() const
{
	return OutOfBlood;
}

