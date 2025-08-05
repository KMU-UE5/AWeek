// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "AWeekGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AAWeekGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AAWeekGameStateBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
