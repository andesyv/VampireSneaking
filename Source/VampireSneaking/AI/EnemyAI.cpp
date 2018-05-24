// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EnemyAI.h"
#include "Enemy.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/CustomAIPerceptionComponent.h"
#include "Player/PlayableCharacterBase.h"
#include "TimerManager.h"
#include "HealthComponent.h"
#include "Player/CustomPlayerController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense.h"
#include "Components/CapsuleComponent.h"

AEnemyAI::AEnemyAI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Making a default blackboard and behavior tree. (A behavior tree inherits from BrainComponent)
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree"));

	// Make health component.
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));

	// Add death event.
	if (HealthComponent) {
		HealthComponent->OnDeath.AddDynamic(this, &AEnemyAI::Death);
	}

	// Assign to Team 1
    SetGenericTeamId(FGenericTeamId(1));
}

AIState AEnemyAI::GetState() const
{
	UBehaviorTreeComponent *behaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (behaviorTreeComp && behaviorTreeComp->GetBlackboardComponent())
	{
		return static_cast<AIState>(behaviorTreeComp->GetBlackboardComponent()->GetValueAsEnum(TEXT("State")));
	}
	return AIState::NoState;
}

void AEnemyAI::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) {
	Super::OnMoveCompleted(RequestID, Result);

	UBehaviorTreeComponent *behaviorTree = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (behaviorTree) {
		OnMoveCompletedDelegate.Broadcast(behaviorTree);
	}
}

void AEnemyAI::Possess(APawn *Pawn) {

	Super::Possess(Pawn);

	// Fetch reference to behaviorTreeComponent and enemy.
	UBehaviorTreeComponent *behaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	AEnemy *possessedEnemy = Cast<AEnemy>(Pawn);

	// Setup blackboard and behavior tree.
	if (possessedEnemy && behaviorTreeComp) {
		if (possessedEnemy->BehaviorTree) {
			if (possessedEnemy->BehaviorTree->BlackboardAsset) {
				
				// Setup blackboard.
				Blackboard->InitializeBlackboard(*possessedEnemy->BehaviorTree->BlackboardAsset);

				// Setup behavior tree.
				behaviorTreeComp->StartTree(*possessedEnemy->BehaviorTree);

				// Setup first control point.
				if (!Blackboard->SetValue<UBlackboardKeyType_Vector>(TEXT("NextPoint"), Pawn->GetActorLocation())) {
					UE_LOG(LogTemp, Warning, TEXT("Failed to set start patrol point for EnemyAI: %s"), *GetName());
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Enemy behavior tree is missing a blackboard!"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("Enemy is missing behavior tree!"));
		}
	} else {
		UE_LOG(LogTemp, Error, TEXT("Missing either something to possess or a behavior tree!"));
	}
}

void AEnemyAI::BeginPlay() {
	Super::BeginPlay();

	TArray<UCustomAIPerceptionComponent*> comps{};
	GetComponents<UCustomAIPerceptionComponent>(comps);
	for (auto item : comps) {
		if (item) {
			AIPerceptionComp = item;
		}
	}
	
	if (GetPerceptionComp()) {
		GetPerceptionComp()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAI::UpdateState);
		GetPerceptionComp()->OnStimulusExpired.AddDynamic(this, &AEnemyAI::StimulusExpired);

		// Setup sight config.
		FAISenseID Id = UAISense::GetSenseID(UAISense_Sight::StaticClass());
		
		if (!Id.IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
			return;
		}

		SightConfig = GetPerceptionComp()->GetSenseConfig(Id);
		
		auto *sight = Cast<UAISenseConfig_Sight>(SightConfig);
		if (sight)
		{
			DefaultVisionRange = sight->SightRadius;
			// LoseRange = sight->LoseSightRadius - DefaultVisionRange;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot find sightConfig!"));
		}
	}
}

UCustomAIPerceptionComponent* AEnemyAI::GetPerceptionComp() const
{
	if (AIPerceptionComp)
	{
		return AIPerceptionComp;
	}
	return nullptr;
}

void AEnemyAI::UnPossess()
{
	Super::UnPossess();

	// Stop behavior tree.
	UBehaviorTreeComponent *behaviorTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (behaviorTreeComp) {
		behaviorTreeComp->StopTree();
	}
}

bool AEnemyAI::ToggleVisionRange() const
{
	UAISenseConfig_Sight *sight = Cast<UAISenseConfig_Sight>(SightConfig);
	if (sight)
	{
		// The enemies should'nt have a harder time loosing sight when in batmode.
		// sight->LoseSightRadius = sight->SightRadius == DefaultVisionRange ? (DefaultVisionRange + LoseRange) / 2 : DefaultVisionRange + LoseRange;
		sight->SightRadius = sight->SightRadius == DefaultVisionRange ? DefaultVisionRange / 2 : DefaultVisionRange;

		if (GetPerceptionComp())
		{
			GetPerceptionComp()->RequestStimuliListenerUpdate();
			return true;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Cannot find sightConfig!"));
	return false;
}

void AEnemyAI::SetLastSeenPosition(AActor* Actor)
{
	if (GetWorld() && DelayedLastSeenPosition.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayedLastSeenPosition);
	}

	if (Actor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetLastSeenPosition received nullptr!"));
		return;
	}

	if (!Blackboard->SetValue<UBlackboardKeyType_Vector>(TEXT("LastSeenPosition"), Actor->GetActorLocation()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard LastSeenPosition."));
	}
}

void AEnemyAI::SetAIIdleState() const
{
	if (Blackboard) {
		if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(AIState::Idle))) {
			UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Missing blackboard!."));
	}
}

void AEnemyAI::ClearTimer(FTimerHandle &timerHandle) const
{
	if (GetWorld() && timerHandle.IsValid() && GetWorld()->GetTimerManager().IsTimerActive(timerHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
	}
}

void AEnemyAI::AddRemoveTargetingEnemy(AddRemoveMode mode, AActor* playerPtr)
{
	if (mode == AddRemoveMode::NoAction)
	{
		return;
	}

	APlayableCharacterBase *player = Cast<APlayableCharacterBase>(playerPtr);
	if (player && player->GetController())
	{
		ACustomPlayerController *playerController = Cast<ACustomPlayerController>(player->GetController());
		if (playerController)
		{
			if (mode == AddRemoveMode::Add) {
				if (playerController->EnemiesTargeting.Contains(this))
				{
					return;
				}
				playerController->EnemiesTargeting.Add(this);
			}
			else
			{
				if (!playerController->EnemiesTargeting.Contains(this))
				{
					return;
				}
				playerController->EnemiesTargeting.Remove(this);
			}
		}
	}
}

void AEnemyAI::UpdateState(const TArray<AActor*>& UpdatedActors)
{
	for (auto item : UpdatedActors)
	{
		if (!item->IsA(APlayableCharacterBase::StaticClass()))
		{
			continue;
		}

		FActorPerceptionBlueprintInfo perceivedInfo;
		if (item && GetPerceptionComp() && GetPerceptionComp()->GetActorsPerception(item, perceivedInfo))
		{
			if (perceivedInfo.LastSensedStimuli.Num() > 0)
			{
				if (Blackboard)
				{
					for (const auto& stimulus : perceivedInfo.LastSensedStimuli)
					{
						FAISenseID SightConfig = UAISense::GetSenseID(UAISense_Sight::StaticClass());
						FAISenseID HearingConfig = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

						if (!SightConfig.IsValid() || !HearingConfig.IsValid())
						{
							UE_LOG(LogTemp, Error, TEXT("Wrong Sense ID"));
							return;
						}
						if (!stimulus.IsValid() || stimulus.IsExpired())
						{
							continue;
						}

						if (stimulus.Type == SightConfig)
						{
							if (stimulus.WasSuccessfullySensed())
							{
								if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(AIState::Combat)))
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
								}

								if (!TargetedActors.Contains(item))
								{
									TargetedActors.Add(item);
								}

								if (!Blackboard->SetValue<UBlackboardKeyType_Object>(TEXT("TargetActor"), item))
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard TargetActor."));
								}

								AddRemoveTargetingEnemy(AddRemoveMode::Add, item);
							}
							else if (GetLengthBetween(item, GetPawn()) > TrueVisionRadius)
							{
								if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(AIState::Searching)))
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
								}

								if (TargetedActors.Contains(item))
								{
									TargetedActors.Remove(item);
								}

								if (!Blackboard->SetValue<UBlackboardKeyType_Object>(TEXT("TargetActor"), nullptr))
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard TargetActor."));
								}

								// Can be stimulis position, but would'nt really change much.
								if (!Blackboard->SetValue<UBlackboardKeyType_Vector>(TEXT("LastSeenPosition"), item->GetActorLocation()))
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard LastSeenPosition."));
								}

								// Set a timer to the delayed last seen position.
								FTimerDelegate TimerDelegate;
								TimerDelegate.BindUFunction(this, FName("SetLastSeenPosition"), item);

								if (GetWorld())
								{
									GetWorld()->GetTimerManager().SetTimer(DelayedLastSeenPosition, TimerDelegate, DelayedLastPositionTime, false);
								}

								AddRemoveTargetingEnemy(AddRemoveMode::Add, item);
							}
							else
							{
								// UE_LOG(LogTemp, Warning, TEXT("Player is lost but inside True vision field!"));
								if (GetWorld())
								{
									GetWorld()->GetTimerManager().SetTimer(VisionRangeTimerHandle, this, &AEnemyAI::CheckIfOutsideVisionRange,
									                                       0.3f, true);
								}
							}
						}
						else if (stimulus.Type == HearingConfig)
						{
							if (static_cast<AIState>(Blackboard->GetValue<UBlackboardKeyType_Enum>(TEXT("State"))) != AIState::Combat)
							{
								if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(AIState::Searching)))
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
								}

								if (!Blackboard->SetValue<UBlackboardKeyType_Vector>(TEXT("LastSeenPosition"), stimulus.StimulusLocation))
								{
									UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard LastSeenPosition."));
								}
							}
						}
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Missing blackboard!."));
				}
			}
		}
	}
}

void AEnemyAI::StimulusExpired(FAIStimulus & stimulus)
{
	if (GetPerceptionComp() == nullptr)
	{
		return;
	}

	const FAISenseID SightConfigId = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	const FAISenseID HearingConfigId = UAISense::GetSenseID(UAISense_Hearing::StaticClass());

	TArray<AActor*> ActorsPerceived{};
	GetPerceptionComp()->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), ActorsPerceived);

	for (auto actor : ActorsPerceived)
	{		
		FActorPerceptionBlueprintInfo perceivedInfo;
		GetPerceptionComp()->GetActorsPerception(actor, perceivedInfo);

		for (auto item : perceivedInfo.LastSensedStimuli)
		{
			// If it's invalid, it means it hasn't been instantiated yet,
			// which means it's definetively not still being searched for.
			if (!item.IsValid())
			{
				continue;
			}
			
			// If even one of the actors are still being searched for, cancel out.
			if ((stimulus.Type == SightConfigId && item.Type == HearingConfigId) || (stimulus.Type == HearingConfigId && item.Type == SightConfigId))
			{
				if (!item.IsExpired())
				{
					UE_LOG(LogTemp, Warning, TEXT("One of the stimuli is still active, aborting."))
					return;
				}
			}
		}
	}

	// If both the sight stimuli and the hearing stimuli for this AI has expired, set it's state to Idle.
	SetAIIdleState();
}

float AEnemyAI::GetLengthBetween(AActor * first, AActor * second)
{
	if (first != nullptr && second != nullptr && !first->IsActorBeingDestroyed() && !second->IsActorBeingDestroyed())
	{
		FVector betweenVector{ first->GetActorLocation() - second->GetActorLocation() };
		return betweenVector.Size();
	}
	return -1.f;
}

void AEnemyAI::CheckIfOutsideVisionRange()
{
	for (auto item : TargetedActors)
	{
		if (item->IsActorBeingDestroyed())
		{
			continue;
		}
		if (GetPawn() && GetLengthBetween(item, GetPawn()) > TrueVisionRadius)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Player is outside vision range and also lost!"));
			const TArray<AActor*> ArrayCopy{ TargetedActors };
			UpdateState(ArrayCopy);
			break;
		}
	}
}

bool AEnemyAI::ToggleSucking() const
{
	if (Blackboard) {
		if (static_cast<AIState>(Blackboard->GetValue<UBlackboardKeyType_Enum>(TEXT("State"))) != AIState::Combat) {
			const bool StateIsFrozen{ lastState != AIState::Frozen && static_cast<AIState>(Blackboard->GetValue<UBlackboardKeyType_Enum>(TEXT("State"))) == AIState::Frozen };
			if (!Blackboard->SetValue<UBlackboardKeyType_Enum>(TEXT("State"), static_cast<int>(StateIsFrozen ? AIState::Idle : AIState::Frozen))) {
				UE_LOG(LogTemp, Warning, TEXT("Failed to set blackboard state enum."));
				return false;
			}
			return !StateIsFrozen;
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Missing blackboard!."));
		return false;
	}
	return false;
}

void AEnemyAI::Death_Implementation() {
	if (GetPawn()) {
		// enemy->Destroy();
		AEnemy *enemy = Cast<AEnemy>(GetPawn());
		if (enemy)
		{
			enemy->dead = true;
			if (enemy->GetCapsuleComponent())
			{
				enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}

		UnPossess();
	}

	if (GetWorld())
	{
		AddRemoveTargetingEnemy(AddRemoveMode::Remove, UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	}
	Destroy();
}