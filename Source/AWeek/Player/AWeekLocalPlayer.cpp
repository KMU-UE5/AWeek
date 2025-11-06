// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekLocalPlayer.h"

#include "AWeek/Settings/AWeekGameUserSettings.h"

UAWeekLocalPlayer::UAWeekLocalPlayer(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
}

UAWeekGameUserSettings* UAWeekLocalPlayer::GetGameUserSettings()
{
	return UAWeekGameUserSettings::Get();
}
