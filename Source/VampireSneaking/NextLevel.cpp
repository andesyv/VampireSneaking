// Fill out your copyright notice in the Description page of Project Settings.

#include "NextLevel.h"
#include "Components/BoxComponent.h"
#include "VampireSneakingGameModeBase.h"
#include "Player/PlayableCharacterBase.h"
#include "Engine/World.h"
#include "Player/CustomPlayerController.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANextLevel::ANextLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box collider"));
	RootComponent = BoxCollider;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ANextLevel::OnHit);

}

// Called when the game starts or when spawned
void ANextLevel::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANextLevel::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(APlayableCharacterBase::StaticClass()) && GetWorld() && Cast<APawn>(OtherActor) && Cast<APawn>(OtherActor)->GetController())
	{
		auto *playerController = Cast<ACustomPlayerController>(Cast<APawn>(OtherActor)->GetController());
		if (playerController)
		{
			UWorld* TheWorld = GetWorld();
			FString CurrentLevel = TheWorld->GetMapName();
			if (CurrentLevel == "AlfaMap") {
				UGameplayStatics::OpenLevel(GetWorld(), "BetaMap");
				//UE_LOG(LogTemp, Warning, TEXT("Loading %s"), *CurrentLevel);
			}
			else {
				UGameplayStatics::OpenLevel(GetWorld(), "AlfaMap");
				//UE_LOG(LogTemp, Warning, TEXT("Loading %s"), *CurrentLevel);
			}
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
void ANextLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

