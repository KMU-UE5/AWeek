// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingItem.h"
#include "SettingItemCategory.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingItemCategory : public USettingItem
{
	GENERATED_BODY()
public:
	void AddSetting(USettingItem* Setting);

	virtual TArray<USettingItem*> GetSettings() override {return Settings; }
protected:
	UPROPERTY()
	TArray<TObjectPtr<USettingItem>> Settings;
};
