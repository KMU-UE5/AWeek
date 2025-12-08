// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AWeekPlayerState.generated.h"

UCLASS()
class AWEEK_API AAWeekPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AAWeekPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};


