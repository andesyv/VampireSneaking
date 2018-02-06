// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVamp.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerVamp::APlayerVamp()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	RootComponent = collider;

	mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Static mesh"));
	mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerVamp::BeginPlay()
{
	Super::BeginPlay();
	
	/*if (!mesh->GetStaticMesh()) {
		UE_LOG(LogTemp, Error, TEXT("No mesh attached to player class instance!"));
	}*/
}

// Called every frame
void APlayerVamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set velocity via physics
	if (collider->IsSimulatingPhysics() && velocity.Size() > KINDA_SMALL_NUMBER) {
		collider->SetPhysicsLinearVelocity(GetActorForwardVector().Rotation().Quaternion() * FVector { velocity.GetClampedToMaxSize(1.f) * MoveSpeed  + GetVelocity().Z});
	}
	//else if (collider->IsSimulatingPhysics() && collider->GetPhysicsLinearVelocity().Size() > 1.f) {
	//	// Adding custom linear drag. (Why didn't I just make a physics material? No idea.)
	//	UE_LOG(LogTemp, Warning, TEXT("Added %f force!"), (-collider->GetPhysicsLinearVelocity() * CustomLinearDrag).Size());
	//	collider->AddForce(-collider->GetPhysicsLinearVelocity() * CustomLinearDrag);
	//}

	// Rotation
	if (controller) {
		Rotate();
	}
}

// Called to bind functionality to input
void APlayerVamp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("XAxis", this, &APlayerVamp::MoveX);
	PlayerInputComponent->BindAxis("YAxis", this, &APlayerVamp::MoveY);

	controller = Cast<APlayerController>(GetController());
	if (controller) {
		controller->bShowMouseCursor = true;
	}
}

void APlayerVamp::MoveX(float amount)
{
	velocity.X = amount;
}

void APlayerVamp::MoveY(float amount)
{
	velocity.Y = amount;
}

void APlayerVamp::Rotate()
{
	FHitResult hitResult{};
	if (controller->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, hitResult)) {
		FVector direction{hitResult.ImpactPoint - GetActorLocation()};
		direction.Z = 0;
		mesh->SetWorldRotation(FRotator{ direction.Rotation() + AdjustmentRotation});
	}
}
