// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerVamp.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Enemy.h"


// Sets default values
APlayerVamp::APlayerVamp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerVamp::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerVamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FName TraceTag("SuckTrace");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	FCollisionQueryParams collisionQueryParams(TraceTag, false);

	FHitResult hitResult{};
	if (GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation() + 100.f, GetActorLocation() + meshComponent->GetForwardVector()*500.f, ECollisionChannel::ECC_WorldDynamic, collisionQueryParams)) {
		AEnemy* enemy = Cast<AEnemy>(hitResult.Actor.Get());
		if (hitResult.Actor.Get()){//->GetActorLocation<GetActorLocation() + meshComponent->GetForwardVector()*500.f) {
			
			Blood += 10.f;


			
		}


		bool enemyClose = false;
	}
}

// Called to bind functionality to input
void APlayerVamp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Bite", IE_Pressed, this, &APlayerVamp::SuckBlood);
}

void APlayerVamp::SuckBlood() {
	UE_LOG(LogTemp, Warning, TEXT(" You pressed the button"));
	APlayableCharacterBase::Blood += 10;
}