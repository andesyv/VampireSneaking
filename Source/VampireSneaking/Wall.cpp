// Fill out your copyright notice in the Description page of Project Settings.

#include "Wall.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Make cube and make it root.
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube"));
	RootComponent = CubeMesh;

	// Add basic cube shape to the meshComponent
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (MeshAsset.Succeeded())
	{
		UStaticMesh* Asset = MeshAsset.Object;

		CubeMesh->SetStaticMesh(Asset);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ObjectFinder failed to find basic cube!"));
	}
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
	// DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	DynamicMaterial = CubeMesh->CreateDynamicMaterialInstance(0);

	TArray<FMaterialParameterInfo> MaterialParameters;
	TArray<FGuid> ParameterIds;
	DynamicMaterial->GetAllScalarParameterInfo(MaterialParameters, ParameterIds);
	UE_LOG(LogTemp, Warning, TEXT("This is %s"), *DynamicMaterial->GetFName().ToString());
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentFadeStatus != FadeState::Finished)
	{
		TickMaterial(DeltaTime);
	}
}

void AWall::SetMaterialVisible(bool Visibility)
{
	CurrentFadeStatus = (Visibility) ? FadeState::Showing : FadeState::Fading;
}

void AWall::TickMaterial(float DeltaTime)
{
	if (DynamicMaterial == nullptr)
	{
		return;
	}

	// Add temporary amount of visibility.
	float newValue{ fadeValue + ((CurrentFadeStatus == FadeState::Showing) ? -FadeSpeed * DeltaTime : FadeSpeed * DeltaTime)};

	UE_LOG(LogTemp, Warning, TEXT("newValue is %f"), newValue);
	switch (CurrentFadeStatus)
	{
	case FadeState::Finished:
		UE_LOG(LogTemp, Warning, TEXT("Currently done!"));
		break;
	case FadeState::Fading:
		UE_LOG(LogTemp, Warning, TEXT("Currently fading!"));
		break;
	case FadeState::Showing:
		UE_LOG(LogTemp, Warning, TEXT("Currently showing!"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("What?!"));
	}

	// Clamp amount between 0 and 1.
	if (newValue > 1.f || newValue < 0.f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Clamped!"));
		newValue = (newValue > 1.f) ? 1.f : 0.f;
		CurrentFadeStatus = FadeState::Finished;
	}
	/*
	if (newValue > 1.f)
	{
		newValue = 1.f;
		CurrentFadeStatus = FadeState::Finished;
	}
	else if (newValue < 0.f)
	{
		newValue = 0.f;
		CurrentFadeStatus = FadeState::Finished;
	}*/
	
	// Set fadeValue equal to the new temporary value
	fadeValue = newValue;
	DynamicMaterial->SetScalarParameterValue(TEXT("EffectValue"), fadeValue);
}

