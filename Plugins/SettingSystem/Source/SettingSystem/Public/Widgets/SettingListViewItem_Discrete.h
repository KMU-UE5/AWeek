// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SettingListViewItem.h"
#include "SettingListViewItem_Discrete.generated.h"

class UCommonButtonBase;
class UCommonRotator;
class USettingValueDiscreteItem;
/**
 * 
 */
UCLASS()
class SETTINGSYSTEM_API USettingListViewItem_Discrete : public USettingListViewItem
{
	GENERATED_BODY()

public:
	virtual void Init(USettingItem* InGameSetting) override;

protected:
	virtual void NativeOnInitialized() override;

	virtual void HandleSettingChangedApplied(USettingItem* ChangedSetting, ESettingChangedReason Reason) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnValueChanged(int Index);

	UFUNCTION()
	void HandleRotatorValueChanged(int32 Value, bool bUserInitiated);
	UFUNCTION()
	void HandleNextButtonClicked();
	UFUNCTION()
	void HandlePrevButtonClicked();

	UPROPERTY()
	TObjectPtr<USettingValueDiscreteItem> DiscreteSetting;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonRotator> Rotator;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> NextButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCommonButtonBase> PrevButton;
};
