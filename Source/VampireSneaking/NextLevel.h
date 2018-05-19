// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NextLevel.generated.h"


class UBoxComponent;


UCLASS()
class VAMPIRESNEAKING_API ANextLevel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANextLevel();

	// Box collider
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent *BoxCollider = nullptr;

	// Hit event
	UFUNCTION(BlueprintCallable)
		void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
