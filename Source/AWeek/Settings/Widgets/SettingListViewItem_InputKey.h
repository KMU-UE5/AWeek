// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SettingListViewItem.h"
#include "SettingListViewItem_InputKey.generated.h"

class UInputKeySelector;
class USettingValueItem_Input;
/**
 * 
 */
UCLASS()
class AWEEK_API USettingListViewItem_InputKey : public USettingListViewItem
{
	GENERATED_BODY()
public:
	virtual void Init(USettingItem* InGameSetting) override;
	virtual void NativeOnInitialized() override;
protected:
	UFUNCTION()
	void HandleKeySelected(FInputChord SelectedKey);

	UFUNCTION(BlueprintImplementableEvent)
	void OnKeySelected(FKey Key);
	
	UPROPERTY()
	TObjectPtr<USettingValueItem_Input> InputSetting;

private:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, BlueprintProtected = true, AllowPrivateAccess = true))
	TObjectPtr<UInputKeySelector> KeySelector;
};
