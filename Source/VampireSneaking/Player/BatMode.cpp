// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/BatMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABatMode::ABatMode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BatModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BatModel"));
	BatModel->SetupAttachment(RootComponent);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleSystem->SetupAttachment(BatModel);
}

void ABatMode::PossessedBy(AController * NewController)
{
	if (ParticleSystem)
	{
		ParticleSystem->Activate(true);
	}
}

void ABatMode::UnPossessed()
{
	if (ParticleSystem)
	{
		ParticleSystem->Deactivate();
	}
}

// Called when the game starts or when spawned
void ABatMode::BeginPlay()
{
	Super::BeginPlay();

	// Override the values set in Super.
	meshStartRotation = BatModel->RelativeRotation;
	meshComponent = BatModel;
}