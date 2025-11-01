// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "SettingListView.generated.h"

class USettingWidgetTypeData;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingListView : public UCommonListView
{
	GENERATED_BODY()

	virtual TSubclassOf<UUserWidget> GetDesiredEntryClassForItem(UObject* Item) const override;

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USettingWidgetTypeData> WidgetTypeData;
};
