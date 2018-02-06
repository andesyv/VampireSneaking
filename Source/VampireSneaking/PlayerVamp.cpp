// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVamp.h"
#include "Components/InputComponent.h"


// Sets default values
APlayerVamp::APlayerVamp()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = collider;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerVamp::BeginPlay()
{
	Super::BeginPlay();
	
	if (!mesh->GetStaticMesh()) {
		UE_LOG(LogTemp, Error, TEXT("No mesh attached to player class instance!"));
		// SetActorTickEnabled(false);
	}
	else {
		// mesh->CanEditSimulatePhysics();
		// mesh->SetSimulatePhysics(true);
	}
}

// Called every frame
void APlayerVamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (collider->IsSimulatingPhysics() && velocity.Size() > KINDA_SMALL_NUMBER) {
		collider->SetPhysicsLinearVelocity(GetActorForwardVector().Rotation().Quaternion() * FVector { velocity.GetClampedToMaxSize(1.f) * MoveSpeed  + GetVelocity().Z});
	}
}

// Called to bind functionality to input
void APlayerVamp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("XAxis", this, &APlayerVamp::MoveX);
	PlayerInputComponent->BindAxis("YAxis", this, &APlayerVamp::MoveY);
}

void APlayerVamp::MoveX(float amount)
{
	velocity.X = amount;
}

void APlayerVamp::MoveY(float amount)
{
	velocity.Y = amount;
}