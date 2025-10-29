// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "AWeekGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UFUNCTION()
	float GetOverallVolume() const;
	UFUNCTION()
	void SetOverallVolume(float InOverallVolume);

	UFUNCTION()
	float GetMusicVolume() const;
	UFUNCTION()
	void SetMusicVolume(float InMusicVolume);
	
private:
	UPROPERTY(Config)
	float OverallVolume = 1.0f;

	UPROPERTY(Config)
	float MusicVolume = 1.0f;
};
