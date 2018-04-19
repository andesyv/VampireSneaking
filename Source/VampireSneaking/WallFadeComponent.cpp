// Fill out your copyright notice in the Description page of Project Settings.

#include "WallFadeComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"


// Sets default values for this component's properties
UWallFadeComponent::UWallFadeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UWallFadeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() == nullptr)
	{
		return;
	}
	
	TArray<UStaticMeshComponent*> StaticMeshes{};
	GetOwner()->GetComponents<UStaticMeshComponent>(StaticMeshes);
	for (auto actor : StaticMeshes)
	{
		if (actor)
		{
			if (actor->GetMaterial(0))
			{
				TArray<FMaterialParameterInfo> ParameterInfos;
				TArray<FGuid> ParameterIds;
				actor->GetMaterial(0)->GetAllScalarParameterInfo(ParameterInfos, ParameterIds);

				for (const auto parameter : ParameterInfos)
				{
					if (parameter.Name == ParameterName)
					{
						DynamicMaterial = actor->CreateDynamicMaterialInstance(0);
						return;
					}
				}
			}
		}
	}
}


// Called every frame
void UWallFadeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentFadeStatus != FadeState::Finished)
	{
		TickMaterial(DeltaTime);
	}
}

void UWallFadeComponent::SetMaterialVisible(bool Visibility)
{
	CurrentFadeStatus = (Visibility) ? FadeState::Showing : FadeState::Fading;
}

void UWallFadeComponent::TickMaterial(float DeltaTime)
{
	if (DynamicMaterial == nullptr)
	{
		return;
	}

	// Add temporary amount of visibility.
	float newValue{ fadeValue + ((CurrentFadeStatus == FadeState::Showing) ? -FadeSpeed * DeltaTime : FadeSpeed * DeltaTime) };

	// Clamp amount between 0 and 1.
	if (newValue > 1.f || newValue < 0.f)
	{
		newValue = (newValue > 1.f) ? 1.f : 0.f;
		CurrentFadeStatus = FadeState::Finished;
	}

	// Set fadeValue equal to the new temporary value
	fadeValue = newValue;
	DynamicMaterial->SetScalarParameterValue(ParameterName, fadeValue);
}
