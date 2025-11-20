// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Inventory/AWeekPlayerHotBar.h"

#include "AWeekItemSlot.h"
#include "AWeek/Components/AWeekPlayerInventoryComponent.h"

#include "Components/UniformGridPanel.h"

void UAWeekPlayerHotBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: PlayerCharacter is null!"), *FString(__FUNCTION__));
		return;
	}
	PlayerInventoryComponent = PlayerCharacter->GetPlayerInventoryComponent();

	for (int32 i = 0; i < PlayerInventoryComponent->GetHotBarInventorySize(); i++)
	{
		UAWeekItemSlot* ItemSlot = CreateWidget<UAWeekItemSlot>(this, ItemSlotClass);
		HotBarGridPanel->AddChildToUniformGrid(ItemSlot, 0, i);
		HotBarSlots.Add(ItemSlot);
	}
	HotBarSlots[0]->SetHighlight(true);

	PlayerInventoryComponent->OnSlotUpdated.AddUObject(this, &UAWeekPlayerHotBar::OnSlotUpdated);
	PlayerInventoryComponent->OnHotbarSelectionChanged.AddUObject(this, &UAWeekPlayerHotBar::OnHotBarSelectionChanged);
	
	RefreshHotBar();
}

void UAWeekPlayerHotBar::RefreshHotBar()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (!PlayerInventoryComponent || !ItemSlotClass)
	{
		return;
	}
	for (int32 i = 0; i < HotBarSlots.Num(); i++)
	{
		HotBarSlots[i]->InitializeItemSlot(PlayerInventoryComponent->GetHotBarItemAt(i).Item);
	}
}

void UAWeekPlayerHotBar::OnHotBarSelectionChanged(int32 OldIndex, int32 NewIndex)
{
	UAWeekItemSlot* OldItemSlot = HotBarSlots[OldIndex];
	UAWeekItemSlot* NewItemSlot = HotBarSlots[NewIndex];
	
	if (OldItemSlot && NewItemSlot)
	{
		OldItemSlot->SetHighlight(false);
		NewItemSlot->SetHighlight(true);
	}
}

void UAWeekPlayerHotBar::OnSlotUpdated(const FAWeekInventorySlotData& SlotData)
{
	if (PlayerInventoryComponent->IsHotBarIndex(SlotData.SlotIndex))
	{
		HotBarSlots[SlotData.SlotIndex]->InitializeItemSlot(SlotData.Item);
	}
}

