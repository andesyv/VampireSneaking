// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerVamp.h"
#include "Kismet/GameplayStatics.h"
#include "AI/EnemyAI.h"
#include "VampireSneakingGameModeBase.h"
#include "Enemy.h"
#include "Player/CustomPlayerController.h"
#include "HealthComponent.h"
#include "Projectile.h"
#include "Math/Vector.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
APlayerVamp::APlayerVamp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	ParticleSystem->SetupAttachment(GetCapsuleComponent());
	ParticleSystem->SetAutoActivate(false);
}

// Called every frame
void APlayerVamp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SuckBlood(SuckSpeed, DeltaTime);


	if (TimeBeforeNextBatToggle > 0.f)
	{
		TimeBeforeNextBatToggle -= DeltaTime; 
	}
	else if (TimeBeforeNextBatToggle < 0.f)
	{
		TimeBeforeNextBatToggle = 0.f;
	}
}

// Called to bind functionality to input
void APlayerVamp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerVamp::AttackCheck);
	PlayerInputComponent->BindAction("BloodAttack", IE_Pressed, this, &APlayerVamp::BloodAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayerVamp::Dash);

	PlayerInputComponent->BindAction("BatTransform", EInputEvent::IE_Pressed, this, &APlayerVamp::BatModeToggle);
}

bool APlayerVamp::ToggleBloodSucking() const
{
	if (suckedEnemy->GetController()) {
		AEnemyAI *enemyAI = Cast<AEnemyAI>(suckedEnemy->GetController());
		if (enemyAI) {
			return enemyAI->ToggleSucking();
		}
	}
	return false;
}

void APlayerVamp::SuckBlood(float amount, float DeltaTime)
{
	// Toggling ability.
	if (GetController()) {
		ACustomPlayerController *playerCon = Cast<ACustomPlayerController>(GetController());
		if (playerCon) {
			if (playerCon->GetBloodSuckButton() && EnemyInFront()) {
				SuckingBlood = !SuckingBlood ? ToggleBloodSucking() : SuckingBlood;
			} else {
				SuckingBlood = SuckingBlood ? ToggleBloodSucking() : SuckingBlood;
			}

			// No need to shorten this logic down even further, just makes it confusing to debug.
			// SuckingBlood = (playerCon->GetBloodSuckButton() && EnemyInFront()) ? (!SuckingBlood ? ToggleBloodSucking() : SuckingBlood) : (SuckingBlood ? ToggleBloodSucking() : SuckingBlood);

			// Do the sucking.
			if (SuckingBlood && suckedEnemy && playerCon->HealthComponent && suckedEnemy->GetController()) {
				playerCon->HealthComponent->AddBlood(amount/5 * DeltaTime);
				AEnemyAI *enemyAI = Cast<AEnemyAI>(suckedEnemy->GetController());
				if (enemyAI && enemyAI->HealthComponent) {
					enemyAI->HealthComponent->AddBlood(-amount * DeltaTime);
				} else {
					UE_LOG(LogTemp, Warning, TEXT("Can't fetch healthcomponent on enemyAI."));
				}				
			} else if (SuckingBlood) {
				UE_LOG(LogTemp, Warning, TEXT("Can't fetch suckedEnemy's controller."));
			}
		}
	}
}

void APlayerVamp::AttackCheck()
{
	if (CHEAT_NoCooldown || TimeBeforeNextAttack <= 0.f)
	{
		Attack();
	}
}

void APlayerVamp::Attack_Implementation()
{
	FHitResult hitResult{}; // TODO: Understand how this can show force in the ApplyPointDamage function.
	

	// Check if enemies are inside players attack area
	if (UGameplayStatics::GetGameMode(GetWorld())) {
		AVampireSneakingGameModeBase *gamemode = Cast<AVampireSneakingGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		if (gamemode) {
			for (auto item : gamemode->GetEnemyList()) {
				if (item == nullptr) {
					continue;
				}

				/// (Exactly the same checks as in "AIVisionCheck.h". Go see there for explanation.
				FVector playerToEnemy{ item->GetActorLocation() - GetActorLocation() };
				if (playerToEnemy.Size() < AttackRange
					&& AVampireSneakingGameModeBase::GetAngleBetween(playerToEnemy, GetMeshForwardVector()) < AttackAngle
					// && !(GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), item->GetActorLocation(), ECollisionChannel::ECC_GameTraceChannel3, collisionQueryParams))
					) {
					UGameplayStatics::ApplyPointDamage(item, AttackDamage, playerToEnemy, hitResult, GetController(), this, DamageType);
				}
			}
		}
	}

	// Set cooldown.
	TimeBeforeNextAttack = AttackCooldown;
}

bool APlayerVamp::EnemyInFront()
{
	const FName TraceTag("SuckTrace");
	GetWorld()->DebugDrawTraceTag = TraceTag;
	const FCollisionQueryParams collisionQueryParams{ TraceTag, false , this };

	FHitResult hitResult{};
	if (GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), GetActorLocation() + GetMeshForwardVector()*100.f, ECollisionChannel::ECC_WorldDynamic, collisionQueryParams))
	{
		AEnemy* enemy = Cast<AEnemy>(hitResult.Actor.Get());
		if (enemy)
		{
			suckedEnemy = enemy;
			return true;
		}
	}
	return false;
}

FVector APlayerVamp::BallisticTrajectory(const FVector &EndPoint)
{
	// The gravity constant g
	const float g{ 980.f };
	FVector distance{ EndPoint - GetActorLocation() };
	// Nullify z as we only need the distance from the ground
	distance.Z = 0;
	// The height from the ground.
	const float h{ GetCapsuleComponent()->GetScaledCapsuleHalfHeight() };

	// I think that calculating VyNegative (which is a - before the last squareroot) would give a result equal to a negative t,
	// which ofcourse doesn't exist in normal world scenarios. Therefore I don't think it is necessary to calculate it.
	const double VyPositive = FMath::Sqrt((2 * g * distance.Size() + FMath::Sqrt(4 * g * distance.Size() + 8 * g * h + 1)) / 2.f);

	return FVector{ static_cast<float>(VyPositive), 0.f, static_cast<float>(VyPositive) };
}

void APlayerVamp::BloodAttack()
{
	if (controller && controller->HealthComponent)
	{
		if (controller->HealthComponent->GetBlood() - BloodProjectileActivationCost > 0.f)
		{
			controller->HealthComponent->AddBlood(-BloodProjectileActivationCost);
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("Not enough blood."));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find controller and/or HealthComponent."));
		return;
	}


	// TODO: Calculate ballistic trajectory for projctile.
	// BallisticTrajectory();

	UWorld* world = GetWorld();
	if (world && GetController())
	{
		APlayerController * playerCon = Cast<APlayerController>(GetController());
		if (playerCon)
		{
			AProjectile *projectile = world->SpawnActor<AProjectile>(ProjectileBlueprint, GetActorLocation()/* + GetMeshForwardVector() * 50.0f*/, GetMeshForwardVector().Rotation());
			
			if (projectile == nullptr)
			{
				return;
			}

			projectile->Instigator = this;
		}
	}
}

void APlayerVamp::BatModeToggle()
{
	// Toggles the batmode Ability.
	if (TimeBeforeNextBatToggle > 0.f)
	{
		return;
	}

	if (GetWorld())
	{
		if (TogglingModes || (BatModeTimerHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(BatModeTimerHandle)))
		{
			// This function should'nt play while it's still being played.
			return;
		}
		GetWorld()->GetTimerManager().SetTimer(BatModeTimerHandle, this, &APlayerVamp::BatModeFinish, BatModeCooldown);
	}
	else
	{
		return;
	}

	BatMode = !BatMode;
	TogglingModes = true;
	TimeBeforeNextBatToggle = BatModeCooldown;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MovementMode = EMovementMode::MOVE_None;
	}

	UE_LOG(LogTemp, Warning, TEXT("Bat mode was toggled!"));
}

void APlayerVamp::BatModeFinish()
{
	TogglingModes = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MovementMode = BatMode ? EMovementMode::MOVE_Flying : EMovementMode::MOVE_Walking;
	}

	if (BatMode)
	{
		ParticleSystem->Activate(true);
	}
	else
	{
		ParticleSystem->Deactivate();
	}
	GetMesh()->ToggleVisibility(false);

	// Toggle vision range of enemies.
	if (!ToggleVisionRanges())
	{
		UE_LOG(LogTemp, Warning, TEXT("Could'nt switch between enemy vision ranges!"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Bat mode was finished toggled."));
}

bool APlayerVamp::ToggleVisionRanges() const
{
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		AVampireSneakingGameModeBase *gamemode = Cast<AVampireSneakingGameModeBase>(GetWorld()->GetAuthGameMode());
		if (gamemode)
		{
			const TArray<AEnemy*> enemies{ gamemode->GetEnemyList() };
			for (auto item : enemies)
			{
				if (item->GetController())
				{
					AEnemyAI *enemyAI = Cast<AEnemyAI>(item->GetController());
					if (enemyAI)
					{
						if (enemyAI->ToggleVisionRange())
						{
							// Continue to skip the return in the loop.
							continue;
						}
					}
				}
				return false;
			}
			// This is clearly the best outcome.
			return true;
		}
	}
	return false;
}

void APlayerVamp::Dash()
{
	if (!CHEAT_NoCooldown && TimeBeforeNextDash > 0.f)
	{
		return;
	}

	FVector AddForce = GetMeshForwardVector() * DashDistance;
	LaunchCharacter(AddForce, false, true);

	// Set cooldown.
	TimeBeforeNextDash = DashCooldown;
}
