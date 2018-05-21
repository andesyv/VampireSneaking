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

	Reset();
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::Die() {
	ded = true;

	// Call death event.
	OnDeath.Broadcast();
}

const float UHealthComponent::TakeDamage(const float amount)
{
	if (CHEAT_Godmode) {
		return Blood;
	}
	return AddBlood(-amount);
}

void UHealthComponent::Reset()
{
	if (MaxBloodIsStartingBlood) {
		Blood = MaxBlood;
	}
	else
	{
		Blood = StartingBlood;
	}
	ded = false;
	OutOfBlood = false;
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

const float UHealthComponent::AddBlood(const float amount)
{
	// If the amount is too small, assume it's 0 and skip out.
	if (FMath::Abs(amount) < KINDA_SMALL_NUMBER || amount == 0.f) {
		return Blood;
	}

	if (CHEAT_Godmode && amount < 0.f) {
		return Blood;
	}

	Blood += amount;
	if (Blood <= 0.f) {
		Blood = 0.f;
		OutOfBlood = true;

		Die();
	} else {
		OutOfBlood = false;
	}

	return Blood;
}

const bool UHealthComponent::IsOutOfBlood() const
{
	return OutOfBlood;
}

