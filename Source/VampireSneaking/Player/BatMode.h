// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/PlayableCharacterBase.h"
#include "BatMode.generated.h"

// Forward declarations
class UStaticMeshComponent;

UCLASS(Blueprintable, meta = (DisplayName = "Bat Mode"))
class VAMPIRESNEAKING_API ABatMode : public APlayableCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABatMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	// Mesh for bat.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *batModel = nullptr;
};
