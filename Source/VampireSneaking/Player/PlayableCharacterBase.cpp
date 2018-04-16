// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayableCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/CustomPlayerController.h"

// Sets default values
APlayableCharacterBase::APlayableCharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TeamId = FGenericTeamId(0);
}

// Called when the game starts or when spawned
void APlayableCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	try {
		TArray<USkeletalMeshComponent*> skeletonMeshes{};
		GetComponents<USkeletalMeshComponent>(skeletonMeshes);
		// check(skeletonMeshes.Num() == 1);
		if (skeletonMeshes.Num() != 1)
		{
			throw FString{ "Skeletonmeshes should equal exactly 1!" };
		}

		meshStartRotation = skeletonMeshes[0]->RelativeRotation;
		meshComponent = skeletonMeshes[0];
	}
	catch (const FString &error)
	{
		UE_LOG(LogTemp, Error, TEXT("Caught error: \"%s\""), *error);
	}
	catch (...)
	{
		UE_LOG(LogTemp, Error, TEXT("Unknown error!"));
	}
}

// Called every frame
void APlayableCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SuckingBlood && controller && meshComponent) {
		Rotate();
	}

	if (TimeBeforeNextAttack < 0.f)
	{
		TimeBeforeNextAttack = 0.f;
	} else if (TimeBeforeNextAttack > 0.f)
	{
		TimeBeforeNextAttack -= DeltaTime;
	}
}

// Called to bind functionality to input
void APlayableCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("XAxis", this, &APlayableCharacterBase::MoveX);
	PlayerInputComponent->BindAxis("YAxis", this, &APlayableCharacterBase::MoveY);

	controller = Cast<ACustomPlayerController>(GetController());
	if (controller) {
		controller->bShowMouseCursor = true;
	}
}

FVector APlayableCharacterBase::GetMeshForwardVector() const
{
	return FVector{(meshComponent->GetForwardVector().Rotation() - meshStartRotation).Vector()};
}

void APlayableCharacterBase::MoveX(float amount)
{
	if (SuckingBlood)
	{
		return;
	}
	AddMovementInput(GetActorRightVector(), amount);
}

void APlayableCharacterBase::MoveY(float amount)
{
	if (SuckingBlood)
	{
		return;
	}
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

FGenericTeamId APlayableCharacterBase::GetGenericTeamId() const
{
	return TeamId;
}
