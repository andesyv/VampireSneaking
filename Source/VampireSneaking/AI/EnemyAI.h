// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAI.generated.h"

// Forward declarations
class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAIPerceptionComponent;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTaskNodeExecutionDelegate, class UBehaviorTreeComponent*, BehaviorTree);

/**
 * Struct for damage info for the behaviorTree task
 */
USTRUCT(BlueprintType)
struct FExplosionDamageInfo {

	GENERATED_BODY()

	float Damage;
	FVector FlingDirection;
	float FlingAmount;

	FExplosionDamageInfo(float _damage = 0.f, const FVector &_flingDirection = FVector{ 0.f, 0.f, 0.f }, float _flingAmount = 1.f)
	 : Damage( _damage ), FlingDirection( _flingDirection ), FlingAmount( _flingAmount )
	{

	}
};

/**
 * The state of the AI, for use in the behaviorTree.
 * (Usable in blueprints and blackboards)
 */
UENUM(BlueprintType)
enum class AIState : uint8 {
	Idle	UMETA(DisplayName = "Idle"),
	Combat	UMETA(DisplayName = "Combat"),
	Searching	UMETA(DisplayName = "Searching"),
	Frozen	UMETA(DisplayName = "Frozen"),
	Attacked	UMETA(DisplayName = "Attacked"),
	
	// When the check function can't give a sensible state.
	NoState	UMETA(DisplayName = "No State"),
};

/**
 * Character controller for AI.
 */
UCLASS(Blueprintable)
class VAMPIRESNEAKING_API AEnemyAI : public AAIController
{
	GENERATED_BODY()

	friend class APlayerVamp;

protected:
	// Start of the game.
	void BeginPlay() override;

	// Called on possession of controller.
	void Possess(APawn *Pawn) override;

	// Called on unpossession of controller.
	void UnPossess() override;

	// Timerhandle for the searching.
	FTimerHandle SearchingTimerHandle;

	// Timerhandle for trueVision range.
	FTimerHandle VisionRangeTimerHandle;

	// Targeted actor
	TArray<AActor*> TargetedActors{};

	// For setting the AI to the Idle state
	UFUNCTION()
	void SetAIIdleState();

	// For clearing a timer.
	void ClearTimer(FTimerHandle& timerHandle) const;

	// Perception component.
	UAIPerceptionComponent *AIPerceptionComp = nullptr;

	// Called whenever AI Perception updates it's state.
	UFUNCTION(BlueprintCallable)
	void UpdateState(const TArray<AActor*> &UpdatedActors);

	float TrueVisionRadius = 150.f;

	static float GetLengthBetween(AActor *first, AActor *second);

	// Check if player is outside trueVision range
	UFUNCTION(BlueprintCallable)
	void CheckIfOutsideVisionRange();

	/**
	 * Toggles the blackboard state enum between Frozen and Idle.
	 * Returns true if state was changed to Frozen.
	 */
	bool ToggleSucking();

	// The state before ToggleSucking. Used in ToggleSucking.
	AIState lastState;

	// Function called on death.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Death();

public:
	// Health and blood component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UHealthComponent *HealthComponent = nullptr;

	AEnemyAI(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Called on completing current movement request */
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	// Delegate for move completion.
	FTaskNodeExecutionDelegate OnMoveCompletedDelegate{};

	UAIPerceptionComponent* const GetPerceptionComp() const;

	// How long the AI will search for before going back into patrol-mode
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SearchTime = 3.f;

	// Info for the next hit taken in behaviorTree.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FExplosionDamageInfo DamageInfo = FExplosionDamageInfo{};
};
