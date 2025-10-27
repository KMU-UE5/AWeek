// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItemCategory.h"

void USettingItemCategory::AddSetting(USettingItem* Setting)
{
	Settings.Add(Setting);
}
