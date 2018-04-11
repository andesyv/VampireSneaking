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
	Blood = GetMaxBlood();

	if (MaxBloodIsStartingBlood) {
		MaxBlood = Blood;
	}
}

void UHealthComponent::Die() {
	ded = true;

	// Call death event.
	OnDeath.Broadcast();
}



const float UHealthComponent::TakeDamage(float amount)
{
	// If the amount is too small, assume it's 0 and skip out.
	if (FMath::Abs(amount) < KINDA_SMALL_NUMBER) {
		return Blood;
	}

	if (CHEAT_Godmode) {
		return Blood;
	}

	Blood -= amount;
	if (Blood <= 0) {
		Blood = 0;
		Die();
	}
	return Blood;
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
	// If the amount is too small, assume it's 0 and skip out.
	if (FMath::Abs(amount) < KINDA_SMALL_NUMBER) {
		return Blood;
	}

	if (CHEAT_InfiniteBlood) {
		return Blood;
	}

	Blood += amount;
	if (Blood <= 0.f) {
		Blood = 0.f;
		OutOfBlood = true;

		if (DieWhenOutOfBlood && !CHEAT_Godmode) {
			Die();
		}
	} else {
		OutOfBlood = false;
	}

	return Blood;
}

const bool UHealthComponent::IsOutOfBlood() const
{
	return OutOfBlood;
}

