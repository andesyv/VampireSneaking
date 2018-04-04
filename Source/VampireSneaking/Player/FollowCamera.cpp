// Fill out your copyright notice in the Description page of Project Settings.

#include "FollowCamera.h"
#include "Camera/CameraComponent.h"


// Sets default values
AFollowCamera::AFollowCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make empty root component
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	// Setup camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called every frame
void AFollowCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		SetActorLocation(Target->GetActorLocation());
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing to follow!"));
	}
}

void AFollowCamera::SetTranslationAndRotation(const FVector & pos, const FRotator & rot)
{
	if (Camera)
	{
		Camera->RelativeLocation = pos;
		Camera->RelativeRotation = rot;
	}
}

