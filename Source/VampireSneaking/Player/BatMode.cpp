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
	Super::BeginPlay();

	// Override the values set in Super.
	meshStartRotation = batModel->RelativeRotation;
	meshComponent = batModel;
}

// Called every frame
void ABatMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (controller && controller->HealthComponent && batModel) {
		// Drain blood while using.
		if (controller->HealthComponent->AddBlood(FMath::Abs(DrainSpeed) * -DeltaTime) < KINDA_SMALL_NUMBER) {
			controller->ChangePawn(0);
		}
	}
}

// Called to bind functionality to input
void ABatMode::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (controller && controller->HealthComponent) {
		controller->HealthComponent->AddBlood(-ActivationCost);
	}
}

