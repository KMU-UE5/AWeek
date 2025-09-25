// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerController.h"

AAWeekPlayerController::AAWeekPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAWeekPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AAWeekPlayerController::Tick(float DeltaTime)
{

}

