// Fill out your copyright notice in the Description page of Project Settings.

#include "DamageType_Explosion.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

void UDamageType_Explosion::HandleDamage(APawn *HitPawn, const FVector &FlingDirection) {
	UDamageType_Explosion *defaultObject = Cast<UDamageType_Explosion>(UDamageType_Explosion::StaticClass()->GetDefaultObject());
	if (defaultObject) {
		HandleDamage(HitPawn, FlingDirection, defaultObject->AppliedForce);
	}
}

void UDamageType_Explosion::HandleDamage(APawn *HitPawn, const FVector &FlingDirection, float FlingForce) {
	if (HitPawn->GetMovementComponent()) {
		UCharacterMovementComponent *moveComp =	Cast<UCharacterMovementComponent>(HitPawn->GetMovementComponent());
		if (moveComp) {
			// moveComp->AddImpulse(FVector{ 10000.f, 0, 0 }, true);
			// moveComp->Velocity += FVector{ 10000.f, 0.f, 0.f };
		}
	}
	ACharacter *Char = Cast<ACharacter>(HitPawn);
	if (Char) {
		Char->LaunchCharacter(FlingDirection.GetSafeNormal() * FlingForce, false, false);
		
	}
	UE_LOG(LogTemp, Warning, TEXT("Damage was handled with %f force!"), FlingForce);
}


