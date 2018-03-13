// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageType_Explosion.h"
#include "GameFramework/Character.h"

void UDamageType_Explosion::HandleDamage(APawn *HitPawn, const FVector &FlingDirection) {
	UDamageType_Explosion *defaultObject = Cast<UDamageType_Explosion>(UDamageType_Explosion::StaticClass()->GetDefaultObject());
	if (defaultObject) {
		HandleDamage(HitPawn, FlingDirection, defaultObject->AppliedForce);
	}
}

void UDamageType_Explosion::HandleDamage(APawn *HitPawn, const FVector &FlingDirection, float FlingForce) {
	ACharacter *Char = Cast<ACharacter>(HitPawn);
	if (Char) {
		Char->LaunchCharacter(FlingDirection.GetSafeNormal() * FlingForce, false, false);
		
	}
	// UE_LOG(LogTemp, Warning, TEXT("Damage was handled with %f force!"), FlingForce);
}


