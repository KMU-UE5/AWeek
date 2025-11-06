// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonLocalPlayer.h"
#include "Engine/LocalPlayer.h"
#include "AWeekLocalPlayer.generated.h"

class UAWeekGameUserSettings;
/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekLocalPlayer : public UCommonLocalPlayer
{
	GENERATED_BODY()
public:
	UAWeekLocalPlayer(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	UAWeekGameUserSettings* GetGameUserSettings();
};
