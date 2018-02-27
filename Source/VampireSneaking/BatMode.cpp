// Fill out your copyright notice in the Description page of Project Settings.

#include "BatMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CustomPlayerController.h"


// Sets default values
ABatMode::ABatMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	batModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatModel"));
	batModel->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABatMode::BeginPlay()
{
	ACharacter::BeginPlay();

	meshStartRotation = batModel->RelativeRotation;
	/*if (GetMovementComponent()) {
		UCharacterMovementComponent *moveComponent = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (moveComponent) {
			moveComponent->MovementMode = EMovementMode::MOVE_Flying;
		}
	}*/
}

void ABatMode::Rotate()
{
	FHitResult hitResult{};
	if (controller->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, hitResult) && batModel) {
		FVector direction{ hitResult.ImpactPoint - GetActorLocation() };
		direction.Z = 0;
		batModel->SetWorldRotation(FRotator{ direction.Rotation() + meshStartRotation });
	}
}

// Called every frame
void ABatMode::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);

	if (controller && batModel) {
		// Drain blood while using.
		AddBlood(-1.f);
		
		Rotate();
	}
}

// Called to bind functionality to input
void ABatMode::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

