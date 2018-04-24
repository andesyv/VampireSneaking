// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "Player/PlayableCharacterBase.h"
#include "Engine/World.h"
#include "Player/CustomPlayerController.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box collider"));
	RootComponent = BoxCollider;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnHit);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACheckpoint::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(APlayableCharacterBase::StaticClass()) && GetWorld() && Cast<APawn>(OtherActor) && Cast<APawn>(OtherActor)->GetController())
	{
		auto *playerController = Cast<ACustomPlayerController>(Cast<APawn>(OtherActor)->GetController());
		if (playerController)
		{
			playerController->LastCheckpoint = this;
		}
		/*auto *gamemode = GetWorld()->GetAuthGameMode<AVampireSneakingGameModeBase>();
		if (gamemode && playerController)
		{
			auto *playerStart = gamemode->FindPlayerStart(playerController);
			if (playerStart)
			{
				playerStart->SetActorLocation(FVector{ playerStart->GetActorLocation().Z, GetActorLocation().Y, GetActorLocation().Z });
				Destroy();
			}
		}*/
	}
	
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

