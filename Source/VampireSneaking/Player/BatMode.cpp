// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/BatMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/CustomPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "HealthComponent.h"


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

	if (Cast<ACustomPlayerController>(GetController()) && controller->HealthComponent && batModel) {
		// Drain blood while using.
		if (controller->HealthComponent->AddBlood(FMath::Abs(DrainSpeed) * -DeltaTime) < KINDA_SMALL_NUMBER) {
			controller->ChangePawn(0);
		}
		
		Rotate();
	}
}

// Called to bind functionality to input
void ABatMode::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ACustomPlayerController *playerController = Cast<ACustomPlayerController>(GetController());
	if (playerController && playerController->HealthComponent) {
		playerController->HealthComponent->AddBlood(-ActivationCost);
	}
}

