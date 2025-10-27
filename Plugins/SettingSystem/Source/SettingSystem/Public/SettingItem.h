// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingItem.generated.h"

/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingItem : public UObject
{
	GENERATED_BODY()
public:
	DECLARE_EVENT_OneParam(USettingItem, FOnSettingChanged, USettingItem*);
	DECLARE_EVENT_OneParam(USettingItem, FOnSettingApplied, USettingItem*);

	FOnSettingChanged OnSettingChangedEvent;
	FOnSettingApplied OnSettingAppliedEvent;

	void NotifySettingChanged();
	void Apply();

	virtual TArray<USettingItem*> GetSettings() { return TArray<USettingItem*>(); }
public:
	UFUNCTION(BlueprintCallable)
	FName GetDevName() const { return DevName; }

	void SetDevName(const FName& InDevName) { DevName = InDevName; }

	UFUNCTION(BlueprintCallable)
	FText GetDisplayName() const { return DisplayName; }

	void SetDisplayName(const FText& InDisplayName) { DisplayName = InDisplayName; }

protected:
	FName DevName;
	FText DisplayName;
};


