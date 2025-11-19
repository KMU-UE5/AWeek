// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekSettingRegistry.h"

#include "AWeekGameUserSettings.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "SettingItemCategory.h"
#include "SettingPropertyResolver.h"
#include "SettingValueDiscreteItem_Bool.h"
#include "SettingValueDiscreteItem_Enum.h"
#include "SettingValueDiscreteItem_Num.h"
#include "SettingValueScalarItem.h"
#include "AWeek/Player/AWeekLocalPlayer.h"
#include "Custom/SettingValueItem_Input.h"
#include "Custom/SettingValueItem_VideoQuality.h"
#include "UserSettings/EnhancedInputUserSettings.h"

#define LOCTEXT_NAMESPACE "AWeek"

#define GET_GAME_SETTINGS_PATH(InLocalPlayer, Name) MakeShared<FSettingPropertyResolver>(InLocalPlayer, TArray<FString>({ \
	GET_FUNCTION_NAME_STRING_CHECKED(UAWeekLocalPlayer, GetGameUserSettings), \
	GET_FUNCTION_NAME_STRING_CHECKED(UAWeekGameUserSettings, Name)}))

void UAWeekSettingRegistry::Init(ULocalPlayer* InLocalPlayer)
{
	Super::Init(InLocalPlayer);

	OwningLocalPlayer = InLocalPlayer;
	GameplaySetting = RegisterGameplaySetting();
	RegisterSetting(GameplaySetting);
	
	GraphicsSetting = RegisterGraphicSetting();
	RegisterSetting(GraphicsSetting);
	
	AudioSetting = RegisterAudioSetting();
	RegisterSetting(AudioSetting);

	KeyboardAndMouseSetting = RegisterKeyboardAndMouseSetting();
	RegisterSetting(KeyboardAndMouseSetting);
}

void UAWeekSettingRegistry::Apply()
{
	Super::Apply();
	if (UAWeekLocalPlayer* LocalPlayer = Cast<UAWeekLocalPlayer>(OwningLocalPlayer))
	{
		LocalPlayer->GetGameUserSettings()->ApplySettings(false);
		const UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();
		UserSettings->ApplySettings();
		UserSettings->SaveSettings();
	}
}

void UAWeekSettingRegistry::Cancel()
{
	Super::Cancel();
}

USettingItem* UAWeekSettingRegistry::RegisterGameplaySetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("GameplayCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_GameplayCategory","Gameplay"));

	USettingValueScalarItem* MouseSensitivityX = NewObject<USettingValueScalarItem>();
	MouseSensitivityX->SetDevName(TEXT("MouseSensitivityX"));
	MouseSensitivityX->SetDisplayName(LOCTEXT("NAME_MouseSensitivityX","MouseSensitivityX"));
	MouseSensitivityX->SetMinValue(0.0f);
	MouseSensitivityX->SetMaxValue(2.0f);
	MouseSensitivityX->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetMouseSensitivityX());
	MouseSensitivityX->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMouseSensitivityX));
	MouseSensitivityX->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMouseSensitivityX));
	Setting->AddSetting(MouseSensitivityX);

	USettingValueScalarItem* MouseSensitivityY = NewObject<USettingValueScalarItem>();
	MouseSensitivityY->SetDevName(TEXT("MouseSensitivityY"));
	MouseSensitivityY->SetDisplayName(LOCTEXT("NAME_MouseSensitivityY","MouseSensitivityY"));
	MouseSensitivityY->SetMinValue(0.0f);
	MouseSensitivityY->SetMaxValue(2.0f);
	MouseSensitivityY->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetMouseSensitivityY());
	MouseSensitivityY->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMouseSensitivityY));
	MouseSensitivityY->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMouseSensitivityY));
	Setting->AddSetting(MouseSensitivityY);
	
	return Setting;
}

USettingItem* UAWeekSettingRegistry::RegisterGraphicSetting()
{
	const UAWeekGameUserSettings* UserSettings = UAWeekGameUserSettings::Get();
	
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("GraphicsCategory"));
	Setting->SetDisplayName(LOCTEXT("SETTING_GraphicsCategory","Graphics"));
	
	USettingItemCategory* Display = NewObject<USettingItemCategory>();
	Display->SetDevName(TEXT("Display"));
	Display->SetDisplayName(LOCTEXT("SETTING_Display","Display"));
	Setting->AddSetting(Display);
	{
		USettingValueDiscreteItem_Enum* WindowMode = NewObject<USettingValueDiscreteItem_Enum>();
		WindowMode->SetDevName(TEXT("WindowMode"));
		WindowMode->SetDisplayName(LOCTEXT("SETTING_WindowMode","WindowMode"));
		WindowMode->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetFullscreenMode));
		WindowMode->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetFullscreenMode));
		WindowMode->SetDefaultValueEnum(EWindowMode::Fullscreen);
		WindowMode->AddEnumOption(EWindowMode::Fullscreen, LOCTEXT("SETTING_WindowModeFullScreen", "FullScreen"));
		WindowMode->AddEnumOption(EWindowMode::Windowed, LOCTEXT("SETTING_WindowModeWindowed", "Windowed"));
		WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, LOCTEXT("SETTING_WindowModeWindowedFullScreen", "WindowedFullScreen"));
		Display->AddSetting(WindowMode);
	}
	

	USettingItemCategory* Graphics = NewObject<USettingItemCategory>();
	Graphics->SetDevName(TEXT("Graphics"));
	Graphics->SetDisplayName(LOCTEXT("SETTING_Graphics","Graphics"));
	Setting->AddSetting(Graphics);
	{
		USettingValueScalarItem* GammaSetting = NewObject<USettingValueScalarItem>();
		GammaSetting->SetDevName(TEXT("Gamma"));
		GammaSetting->SetDisplayName(LOCTEXT("SETTING_Gamma","Gamma"));
		GammaSetting->SetMinValue(1.0f);
		GammaSetting->SetMaxValue(5.0f);
		GammaSetting->SetDefaultValue(2.2f);
		GammaSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetGamma));
		GammaSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetGamma));
		Graphics->AddSetting(GammaSetting);
	}
	{
		USettingValueItem_VideoQuality* OverallQuality = NewObject<USettingValueItem_VideoQuality>();
		OverallQuality->SetDevName(TEXT("OverallQuality"));
		OverallQuality->SetDisplayName(LOCTEXT("SETTING_OverallQuality","OverallQuality"));
		Graphics->AddSetting(OverallQuality);
	}
	

	return Setting;
}

USettingItem* UAWeekSettingRegistry::RegisterAudioSetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("AudioCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_AudioCategory","Audio"));

	USettingItemCategory* VolumeSetting = NewObject<USettingItemCategory>();
	VolumeSetting->SetDevName(TEXT("VolumeSetting"));
	VolumeSetting->SetDisplayName(LOCTEXT("SETTING_Volume","Volume"));
	Setting->AddSetting(VolumeSetting);
	
	USettingValueScalarItem* OverallSetting = NewObject<USettingValueScalarItem>();
	OverallSetting->SetDevName(TEXT("OverallVolume"));
	OverallSetting->SetDisplayName(LOCTEXT("SETTING_OverallVolume","Overall"));
	OverallSetting->SetMinValue(0.0f);
	OverallSetting->SetMaxValue(2.0f);
	OverallSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetOverallVolume());
	OverallSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetOverallVolume));
	OverallSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetOverallVolume));
	VolumeSetting->AddSetting(OverallSetting);

	USettingValueScalarItem* MusicSetting = NewObject<USettingValueScalarItem>();
	MusicSetting->SetDevName(TEXT("MusicVolume"));
	MusicSetting->SetDisplayName(LOCTEXT("SETTING_MusicVolume","MusicSetting"));
	MusicSetting->SetMinValue(0.0f);
	MusicSetting->SetMaxValue(2.0f);
	MusicSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetMusicVolume());
	MusicSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetMusicVolume));
	MusicSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetMusicVolume));
	VolumeSetting->AddSetting(MusicSetting);

	USettingValueScalarItem* SFXSetting = NewObject<USettingValueScalarItem>();
	SFXSetting->SetDevName(TEXT("SFXVolume"));
	SFXSetting->SetDisplayName(LOCTEXT("SETTING_SFXVolume","SFXSetting"));
	SFXSetting->SetMinValue(0.0f);
	SFXSetting->SetMaxValue(2.0f);
	SFXSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetSFXVolume());
	SFXSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetSFXVolume));
	SFXSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetSFXVolume));
	VolumeSetting->AddSetting(SFXSetting);

	USettingValueScalarItem* UISetting = NewObject<USettingValueScalarItem>();
	UISetting->SetDevName(TEXT("UIVolume"));
	UISetting->SetDisplayName(LOCTEXT("SETTING_UIVolume","UI"));
	UISetting->SetMinValue(0.0f);
	UISetting->SetMaxValue(2.0f);
	UISetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetUIVolume());
	UISetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetUIVolume));
	UISetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetUIVolume));
	VolumeSetting->AddSetting(UISetting);

	USettingValueScalarItem* AmbientSetting = NewObject<USettingValueScalarItem>();
	AmbientSetting->SetDevName(TEXT("AmbientVolume"));
	AmbientSetting->SetDisplayName(LOCTEXT("SETTING_AmbientVolume","Ambient"));
	AmbientSetting->SetMinValue(0.0f);
	AmbientSetting->SetMaxValue(2.0f);
	AmbientSetting->SetDefaultValue(GetDefault<UAWeekGameUserSettings>()->GetAmbientVolume());
	AmbientSetting->SetGetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, GetAmbientVolume));
	AmbientSetting->SetSetter(GET_GAME_SETTINGS_PATH(OwningLocalPlayer, SetAmbientVolume));
	VolumeSetting->AddSetting(AmbientSetting);
	
	return Setting;
}

USettingItem* UAWeekSettingRegistry::RegisterKeyboardAndMouseSetting()
{
	USettingItemCategory* Setting = NewObject<USettingItemCategory>();
	Setting->SetDevName(TEXT("KeyboardAndMouseCategory"));
	Setting->SetDisplayName(LOCTEXT("NAME_KeyboardAndMouseCategory","KeyboardAndMouse"));

	const UEnhancedInputLocalPlayerSubsystem* InputSubsystem = OwningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	UEnhancedInputUserSettings* UserSettings = InputSubsystem->GetUserSettings();
	for (const TPair<FGameplayTag, TObjectPtr<UEnhancedPlayerMappableKeyProfile>>& ProfilePair : UserSettings->GetAllSavedKeyProfiles())
	{
		const FGameplayTag& ProfileName = ProfilePair.Key;
		const TObjectPtr<UEnhancedPlayerMappableKeyProfile>& Profile = ProfilePair.Value;

		for (const TPair<FName, FKeyMappingRow>& RowPair : Profile->GetPlayerMappingRows())
		{
			if (RowPair.Value.HasAnyMappings())
			{
				FPlayerMappableKeyQueryOptions Options = {};
				Options.KeyToMatch = EKeys::A;
				Options.bMatchBasicKeyTypes = true;

				for (const FPlayerKeyMapping& Mapping : RowPair.Value.Mappings)
				{
					if (!Profile->DoesMappingPassQueryOptions(Mapping, Options))
					{
						continue;
					}
					USettingValueItem_Input* InputSetting = NewObject<USettingValueItem_Input>();
					InputSetting->Init(Profile, RowPair.Value, Options, UserSettings);
					Setting->AddSetting(InputSetting);
				}
			}
		}
	}
		

	return Setting;
}
