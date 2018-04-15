// Fill out your copyright notice in the Description page of Project Settings.

#include "FollowCamera.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFollowCamera::AFollowCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make empty root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	// Setup springarm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Root);

	// Setup camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called every frame
void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		FVector newPosition{Target->GetActorLocation()};
		if (LockZ)
		{
			newPosition.Z = GetActorLocation().Z;
		}
		SetActorLocation(newPosition);
	}
}

bool AFollowCamera::ViewBlockingTrace(TArray<FHitResult>& OutHits)
{
	UWorld* world = GetWorld();
	if (world && Target && Camera)
	{
		if (Target->IsA(ACharacter::StaticClass()))
		{
			FCollisionShape SphereShape{};
			auto* Player = Cast<ACharacter>(Target);

			if (Player && Player->GetRootComponent())
			{
				auto* capsule = Cast<UCapsuleComponent>(Player->GetRootComponent());
				if (capsule)
				{
					SphereShape.SetCapsule(capsule->GetScaledCapsuleRadius(), capsule->GetScaledCapsuleHalfHeight());
				}
			}

			return world->SweepMultiByChannel(OutHits, FVector{Camera->RelativeLocation + GetActorLocation()},
			                                  Target->GetActorLocation(), FQuat::Identity, ECC_Camera, SphereShape);
		}
		// If the target isn't the player, just normal trace
		return world->LineTraceMultiByChannel(OutHits, FVector{Camera->RelativeLocation + GetActorLocation()},
		                                      Target->GetActorLocation(), ECC_Camera);
	}
	return false;
}
