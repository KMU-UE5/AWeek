// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekPlayerInventoryPanel.h"

#include "AWeekInventoryItemSlot.h"
#include "AWeekItemSlot.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UAWeekPlayerInventoryPanel::LinkToInventory(const TObjectPtr<UAWeekPlayerInventoryComponent> InPlayerInventoryComponent)
{
	PlayerInventoryComponent = InPlayerInventoryComponent;
	Super::LinkToInventory(InPlayerInventoryComponent);
}

void UAWeekPlayerInventoryPanel::InitializeGridPanel()
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	const int32 HotBarSize = PlayerInventoryComponent->GetHotBarInventorySize();
	const int32 InventorySize = InventoryComponent->GetSlotsCapacity();
	HotBarSlots.Empty();
	for (int32 i = 0; i < InventorySize; i++)
	{
		UAWeekInventoryItemSlot* ItemSlot = CreateWidget<UAWeekInventoryItemSlot>(this, InventorySlotClass);
		ItemSlot->SetInventory(InventoryComponent);
		ItemSlot->SetItemSlotIndex(i);
		if (i < HotBarSize)
		{
			HotBarGridPanel->AddChildToUniformGrid(ItemSlot, 0, i);
			HotBarSlots.Add(ItemSlot);
		}
		else if (i < InventorySize - 1)
		{
			const int32 InventoryGridIndex = i - HotBarSize; 
			InventoryGridPanel->AddChildToUniformGrid(ItemSlot,
				InventoryGridIndex / NumCols, InventoryGridIndex % NumCols);
			InventoryItemSlots.Add(ItemSlot);
		}
		else
		{
			// trash can
			InventoryGridPanel->AddChildToUniformGrid(ItemSlot, (InventorySize-1) / NumCols, NumCols);
			InventoryItemSlots.Add(ItemSlot);
		}
	}
}

void UAWeekPlayerInventoryPanel::RefreshInventoryPanel()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	Super::RefreshInventoryPanel();
	
	if (PlayerInventoryComponent)
	{
		for (int32 i = 0; i < HotBarSlots.Num(); i++)
		{
			HotBarSlots[i]->InitializeInventoryItemSlot(
				PlayerInventoryComponent->GetItemSlotAt(HotBarSlots[i]->GetItemSlotIndex()).Item);
		}
	}
}

void UAWeekPlayerInventoryPanel::OnSlotUpdate(const FAWeekInventorySlotData& SlotData)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: 2 Index=%d"), *FString(__FUNCTION__), SlotData.SlotIndex);
	if (PlayerInventoryComponent->IsHotBarIndex(SlotData.SlotIndex))
	{
		HotBarSlots[SlotData.SlotIndex]->InitializeInventoryItemSlot(SlotData.Item);
	}
	else
	{
		int32 InventorySlotIndex = PlayerInventoryComponent->ToInventorySlotIndex(SlotData.SlotIndex);
		InventoryItemSlots[InventorySlotIndex]->InitializeInventoryItemSlot(SlotData.Item);
	}
	UpdateInfoText();
}

void UAWeekPlayerInventoryPanel::UpdateInfoText() const
{
	const FString WeightInfoValue = FString::Printf(
		TEXT("Weight: %.1f/%.1f"),
		InventoryComponent->GetWeightCapacity(),
		InventoryComponent->GetInventoryTotalWeight()
	);
	const FString CapacityInfoValue = FString::Printf(
		TEXT("Capacity: %d/%d"),
		InventoryComponent->GetSlotsCapacity() - InventoryComponent->GetEmptySlotsNum(),
		InventoryComponent->GetSlotsCapacity()
	);

	WeightInfo->SetText(FText::FromString(WeightInfoValue));
	CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}