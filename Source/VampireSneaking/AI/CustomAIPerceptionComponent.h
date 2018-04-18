// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "CustomAIPerceptionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStimulusExpiredEvent, FAIStimulus&, StimulusStore);

/**
 * A custom implementation of AIPerceptionComponent.
 * Only real difference is that this implementation also has a delegate that calls whenever a stimulus has expired.
 */
UCLASS(ClassGroup = AI, HideCategories = (Activation, Collision), meta = (BlueprintSpawnableComponent), config = Game)
class VAMPIRESNEAKING_API UCustomAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
protected:
	void HandleExpiredStimulus(FAIStimulus& StimulusStore) override;
	
public:
	FStimulusExpiredEvent StimulusExpired;
};
