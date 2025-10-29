// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingRegistry.h"

#include "SettingItemCategory.h"
#include "SettingValueScalarItem.h"

#define LOCTEXT_NAMESPACE "AWeek"

void UAWeekSettingRegistry::Init(ULocalPlayer* InLocalPlayer)
{
	Super::Init(InLocalPlayer);

	AudioSetting = RegisterAudioSetting();
	RootSettings.Add(AudioSetting);
}

USettingItem* UAWeekSettingRegistry::RegisterAudioSetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("AudioCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_AudioCategory","Audio"));

	USettingValueScalarItem* OverallSetting = NewObject<USettingValueScalarItem>();
	OverallSetting->SetDevName(TEXT("OverallSetting"));
	OverallSetting->SetDisplayName(LOCTEXT("NAME_OverallSetting","OverallSetting"));
	OverallSetting->SetMinValue(0.0f);
	OverallSetting->SetMaxValue(1.0f);
	Setting->AddSetting(OverallSetting);
	
	return Setting;
}
