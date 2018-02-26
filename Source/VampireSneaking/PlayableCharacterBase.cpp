// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayableCharacterBase.h"

// Sets default values
APlayableCharacterBase::APlayableCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayableCharacterBase::BeginPlay()
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
void APlayableCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (controller && meshComponent) {
		Rotate();
	}
}

// Called to bind functionality to input
void APlayableCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("XAxis", this, &APlayableCharacterBase::MoveX);
	PlayerInputComponent->BindAxis("YAxis", this, &APlayableCharacterBase::MoveY);

	controller = Cast<APlayerController>(GetController());
	if (controller) {
		controller->bShowMouseCursor = true;
	}
}

void APlayableCharacterBase::MoveX(float amount)
{
	AddMovementInput(GetActorRightVector(), amount);
}

void APlayableCharacterBase::MoveY(float amount)
{
	AddMovementInput(GetActorForwardVector(), amount);
}

void APlayableCharacterBase::Rotate()
{
	FHitResult hitResult{};
	if (controller->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, hitResult) && meshComponent) {
		FVector direction{ hitResult.ImpactPoint - GetActorLocation() };
		direction.Z = 0;
		meshComponent->SetWorldRotation(FRotator{ direction.Rotation() + meshStartRotation });
	}
}

