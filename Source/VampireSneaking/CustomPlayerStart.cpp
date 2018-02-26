// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomPlayerStart.h"

ACustomPlayerStart::ACustomPlayerStart(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("Custom start!"));
}
