// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameUserSettings.h"

UAWeekGameUserSettings* UAWeekGameUserSettings::Get()
{
	return CastChecked<UAWeekGameUserSettings>(GEngine->GetGameUserSettings());
}

float UAWeekGameUserSettings::GetOverallVolume() const
{
	return OverallVolume;
}

void UAWeekGameUserSettings::SetOverallVolume(float InOverallVolume)
{
	OverallVolume = InOverallVolume;
}

float UAWeekGameUserSettings::GetMusicVolume() const
{
	return MusicVolume;
}

void UAWeekGameUserSettings::SetMusicVolume(float InMusicVolume)
{
	MusicVolume = InMusicVolume;
}
