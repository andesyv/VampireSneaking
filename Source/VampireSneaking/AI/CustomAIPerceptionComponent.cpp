// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomAIPerceptionComponent.h"

void UCustomAIPerceptionComponent::HandleExpiredStimulus(FAIStimulus& StimulusStore) {
	
	OnStimulusExpired.Broadcast(StimulusStore);

}


