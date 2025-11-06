// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SettingWidgetTypeData.generated.h"

class USettingListViewItem;
class USettingItem;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingWidgetTypeData : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetListViewItemForSetting(const USettingItem* Setting) const;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (AllowAbstract))
	TMap<TSubclassOf<USettingItem>, TSubclassOf<USettingListViewItem>> EntryWidgetForClass;

};
