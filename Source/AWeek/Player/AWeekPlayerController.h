// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AWeekPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AAWeekPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAWeekPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
