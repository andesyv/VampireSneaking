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

	TArray<USkeletalMeshComponent*> skeletonMeshes{};
	GetComponents<USkeletalMeshComponent>(skeletonMeshes);
	check(skeletonMeshes.Num() == 1 && "Skeletonmeshes should equal exactly 1!");

	meshStartRotation = skeletonMeshes[0]->RelativeRotation;
	meshComponent = skeletonMeshes[0];
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

	if (TimeBeforeNextDash < 0.f)
	{
		TimeBeforeNextDash = 0.f;
	}
	else if (TimeBeforeNextDash > 0.f)
	{
		TimeBeforeNextDash -= DeltaTime;
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

FVector APlayableCharacterBase::GetMouseVector() const
{
	FHitResult hitResult{};
	if (controller && controller->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel2, false, hitResult)) {
		FVector direction{ hitResult.ImpactPoint - GetActorLocation() };
		direction.Z = 0;
		return direction;
	}
	return FVector::ZeroVector;
}

FRotator APlayableCharacterBase::RotateAToB(FVector a, FVector b) const
{
	a = a.GetSafeNormal();
	b = b.GetSafeNormal();
	const FVector v{ FVector::CrossProduct(a, b) };
	const float sinV{ FMath::Abs(v.Size()) };
	const float cosV{ FVector::DotProduct(a, b) };
	const FMatrix Vx{ FVector{0, -v.Z, v.Y}, FVector{v.Z, 0, -v.X}, FVector{-v.Y, v.X, 0}, FVector::ZeroVector};

	const FMatrix rotationMatrix{ FMatrix::Identity + Vx + Vx * Vx * (1.f / (1.f + cosV)) };

	return rotationMatrix.Rotator();
}

FVector APlayableCharacterBase::GetMeshForwardVector() const
{
	if (meshComponent != nullptr)
	{
		return FVector{ (meshComponent->GetForwardVector().Rotation() - meshStartRotation).Vector() };
	}
	return FVector::ZeroVector;
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
	if (meshComponent != nullptr)
	{
		meshComponent->SetWorldRotation(FRotator{ GetMouseVector().Rotation() + meshStartRotation });
	}
}

FGenericTeamId APlayableCharacterBase::GetGenericTeamId() const
{
	return TeamId;
}
