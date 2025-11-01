// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SettingListView.h"

#include "SettingItem.h"
#include "Widgets/SettingWidgetTypeData.h"

TSubclassOf<UUserWidget> USettingListView::GetDesiredEntryClassForItem(UObject* Item) const
{
	if (const USettingItem* Setting = Cast<USettingItem>(Item))
	{
		return WidgetTypeData->GetListViewItemForSetting(Setting);
	}
	return Super::GetDesiredEntryClassForItem(Item);
}
