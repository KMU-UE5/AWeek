// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerState.h"

AAWeekPlayerState::AAWeekPlayerState(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAWeekPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AAWeekPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}