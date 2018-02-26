// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVamp.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIVisionCheck.h"

// Sets default values
APlayerVamp::APlayerVamp()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	// Set health.
	Health = GetMaxHealth();
}

// Called when the game starts or when spawned
void APlayerVamp::BeginPlay()
{
	Super::BeginPlay();

	TArray<USkeletalMeshComponent*> skeletonMeshes;
	GetComponents<USkeletalMeshComponent>(skeletonMeshes);
	if (skeletonMeshes.Num() > 1) {
		UE_LOG(LogTemp, Error, TEXT("This character got %d skeleton meshes!!"), skeletonMeshes.Num());
	}
	else if (skeletonMeshes.Num() < 1) {
		UE_LOG(LogTemp, Error, TEXT("This character got no skeleton mesh!!"));
		return;
	}

	meshStartRotation = skeletonMeshes[0]->RelativeRotation;
	meshComponent = skeletonMeshes[0];
}

// Called every frame
void APlayerVamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (controller && meshComponent) {
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

const float APlayerVamp::GetHealth() const
{
	return Health;
}

const float APlayerVamp::GetMaxHealth() const
{
	return MaxHealth;
}

const float APlayerVamp::GetPercentageHealth() const
{
	return Health / MaxHealth;
}

const float APlayerVamp::GetBlood() const
{
	return Blood;
}

const float APlayerVamp::TakeDamage(float damage)
{
	Health -= damage;
	if (Health <= 0) {
		Health = 0;
		ded = true;
	}
	return Health;
}

void APlayerVamp::MoveX(float amount)
{
	AddMovementInput(GetActorRightVector(), amount);
}

void APlayerVamp::MoveY(float amount)
{
	AddMovementInput(GetActorForwardVector(), amount);
}

void APlayerVamp::Rotate()
{
	FHitResult hitResult{};
	if (controller->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, hitResult) && meshComponent) {
		FVector direction{hitResult.ImpactPoint - GetActorLocation()};
		direction.Z = 0;
		meshComponent->SetWorldRotation(FRotator{ direction.Rotation() + meshStartRotation});
	}
}

void APlayerVamp::OnOverlap(UPrimitiveComponent* OverlappedComponent, ACharacter *OtherCharacter,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Character overlapping, this is working as intended. no fix needed")); //Does not work yet
	if (OtherCharacter->IsA(AEnemy::StaticClass())) {
		//how to do "if current state = idle"?
			Blood += 10.f;
		
	}
	
}