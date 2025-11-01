// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/AWeekGameUIManager.h"
#include "AWeek/UI/Inventory/AWeekInventoryMainPanel.h"
#include "AWeek/UI/Crafting/AWeekCraftingMainPanel.h"
#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"
#include "AWeek/UI/Inventory/AWeekHeldItemVisual.h"
#include "AWeek/UI/Inventory/AWeekHeldItem.h"
#include "AWeek/UI/MainWidget/MainUIWidget.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Items/AWeekItemBase.h"
#include "CommonUIExtensions.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Player/AWeekPlayerController.h"
#include "AWeek/Data/AWeekUIDataAsset.h"
#include "AWeek/Components/AWeekCraftingComponent.h"
#include "Controller/AWeekInventoryController.h"
#include "Controller/AWeekCraftingController.h"

UAWeekGameUIManager::UAWeekGameUIManager()
{
	UIDataAssetPath = FSoftObjectPath(TEXT("/Game/Data/UI/UA_UIClassSettings.UA_UIClassSettings"));
}

void UAWeekGameUIManager::InitializeUIManager(const TObjectPtr<AAWeekPlayerCharacter> InPlayerCharacter)
{
	PlayerController = Cast<AAWeekPlayerController>(GetWorld()->GetFirstPlayerController());
	LocalPlayer = PlayerController->GetLocalPlayer();
	PlayerCharacter = InPlayerCharacter;
	
	if (UIDataAssetPath.IsValid())
	{
		UIDataAsset = Cast<UAWeekUIDataAsset>(UIDataAssetPath.TryLoad());
        
		if (UIDataAsset)
		{
			InventoryMainPanelClass = UIDataAsset->InventoryMainPanelClass;
			CraftingMainPanelClass = UIDataAsset->CraftingMainPanelClass;
			InteractionWidgetClass = UIDataAsset->InteractionWidgetClass;
			HeldItemVisualClass = UIDataAsset->HeldItemVisualClass;
			MainUIWidgetClass = UIDataAsset->MainWidgetClass;
            
			UE_LOG(LogTemp, Log, TEXT("UI DataAsset loaded from: %s"), *UIDataAssetPath.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load UI DataAsset from: %s"), *UIDataAssetPath.ToString());
			return;
		}
	}

	// Create UI Controllers
	InventoryController = NewObject<UAWeekInventoryController>(this, UAWeekInventoryController::StaticClass());
	InventoryController->InitializeInventoryController(this, HeldItemVisualClass);

	CraftingController = NewObject<UAWeekCraftingController>(this, UAWeekCraftingController::StaticClass());
	CraftingController->InitializeCraftingController(this,
		PlayerCharacter->GetCraftingComponent(),
		PlayerCharacter->GetPlayerInventoryComponent());
	
	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
		InventoryMainPanelWidget->InitializeInventoryMainPanel();
		InventoryMainPanelWidget->DeactivateWidget();
	}
}
// =====================================================
// INVENTORY SYSTEM
// =====================================================

void UAWeekGameUIManager::ShowInventoryMainPanel()
{	
	if (InventoryMainPanelClass)
	{
		InventoryMainPanelWidget = Cast<UAWeekInventoryMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), InventoryMainPanelClass));
	}
}

void UAWeekGameUIManager::ShowMainWidget()
{
	UE_LOG(LogTemp, Log, TEXT("1asdf"));
	if (MainUIWidgetClass)
	{
		UE_LOG(LogTemp, Log, TEXT("2asdf"));
		MainUIWidget = Cast<UMainUIWidget, UCommonActivatableWidget>(
		UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
			FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), MainUIWidgetClass));

		UE_LOG(LogTemp, Log, TEXT("MainUIWidgetClass=%s IsChildOf Activatable? %s"),
	*GetNameSafe(*MainUIWidgetClass),
	(MainUIWidgetClass && MainUIWidgetClass->IsChildOf(UCommonActivatableWidget::StaticClass())) ? TEXT("YES") : TEXT("NO"));
	}
}


void UAWeekGameUIManager::HideInventoryMainPanel()
{	
	if (InventoryMainPanelWidget)
	{
		InventoryMainPanelWidget->DeactivateWidget();
		InventoryController->ReturnHeldItemToInventory();
	}
}

void UAWeekGameUIManager::HideMainWidget()
{
	if (MainUIWidget)
	{
		MainUIWidget->DeactivateWidget();
		InventoryController->ReturnHeldItemToInventory();
	}
}

void UAWeekGameUIManager::ShowCraftingMainPanel(const TObjectPtr<UAWeekCraftingComponent> CraftingComponent, const TObjectPtr<UAWeekInventoryComponent> InventoryComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (CraftingMainPanelClass)
	{
		CraftingMainPanelWidget = Cast<UAWeekCraftingMainPanel, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.GameMenu"), CraftingMainPanelClass));
		CraftingComponent->UpdateInventoryCounts();
		CraftingMainPanelWidget->InitializeCraftingMainPanel(CraftingController, InventoryComponent);
	}
}

void UAWeekGameUIManager::HideCraftingMainPanel()
{
	UE_LOG(LogTemp, Warning, TEXT("%s:"), *FString(__FUNCTION__));
	if (CraftingMainPanelWidget)
	{
		CraftingMainPanelWidget->DeactivateWidget();
	}
}

void UAWeekGameUIManager::ToggleInventoryMainPanel()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
	{
		ShowInventoryMainPanel();
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		if (InventoryMainPanelWidget->IsChestOpen())
		{
			DeactivateChestInventory();
		}
		HideInventoryMainPanel();
		PlayerController->SetShowMouseCursor(false);
	}
}
void UAWeekGameUIManager::ToggleMainWidget()
{
	UE_LOG(LogTemp, Log, TEXT("1MainWidget Open"));
	if (!IsValid(MainUIWidget) || !MainUIWidget->IsActivated())
	{
		UE_LOG(LogTemp, Log, TEXT("2MainWidget close"));
		ShowMainWidget();
		PlayerController->SetShowMouseCursor(true);
	}else
	{
		UE_LOG(LogTemp, Log, TEXT("3MainWidget Open"));
		HideMainWidget();
		PlayerController->SetShowMouseCursor(false);
	}
}



void UAWeekGameUIManager::ToggleChestInventory(TObjectPtr<UAWeekInventoryComponent> ChestInventory)
{
	if (!InventoryMainPanelWidget->IsChestOpen())
	{
		if (!IsValid(InventoryMainPanelWidget) || !InventoryMainPanelWidget->IsActivated())
		{
			ShowInventoryMainPanel();
			PlayerController->SetShowMouseCursor(true);
		}
		ActivateChestInventory(ChestInventory);
	}
	else
	{
		DeactivateChestInventory();
		HideInventoryMainPanel();
		PlayerController->SetShowMouseCursor(false);
	}
}

void UAWeekGameUIManager::ToggleCraftingMainPanel(const TObjectPtr<UAWeekCraftingComponent> CraftingComponent, const TObjectPtr<UAWeekInventoryComponent> InventoryComponent)
{
	if (!IsValid(CraftingMainPanelWidget) || !CraftingMainPanelWidget->IsActivated())
	{
		ShowCraftingMainPanel(CraftingComponent, InventoryComponent);
		PlayerController->SetShowMouseCursor(true);
	}
	else
	{
		HideCraftingMainPanel();
		PlayerController->SetShowMouseCursor(false);
	}
}


// void UAWeekGameUIManager::ShowCrosshair()
// {
// 	if (CrosshairWidget)
// 	{
// 		CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
// 	}
// }
//
// void UAWeekGameUIManager::HideCrosshair()
// {
// 	if (CrosshairWidget)
// 	{
// 		CrosshairWidget->SetVisibility(ESlateVisibility::Collapsed);
// 	}
// }

void UAWeekGameUIManager::ShowInteractionWidget()
{
	if (InteractionWidgetClass)
	{
		InteractionWidget = Cast<UAWeekInteractionWidget, UCommonActivatableWidget>(
			UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer,
				FGameplayTag::RequestGameplayTag("UI.Layer.Game"), InteractionWidgetClass));
	}
}
void UAWeekGameUIManager::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->DeactivateWidget();
	}
}
void UAWeekGameUIManager::UpdateInteractionWidget(const FAWeekInteractableData* InteractableData)
{
	if (InteractionWidget == nullptr || !InteractionWidget->IsActivated())
	{
		if (InteractionWidgetClass)
		{
			ShowInteractionWidget();
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void UAWeekGameUIManager::ActivateChestInventory(const TObjectPtr<UAWeekInventoryComponent> ChestInventory) const
{
	PlayerCharacter->SetChestInventoryComponent(ChestInventory);
	InventoryMainPanelWidget->ActivateChestInventory(ChestInventory);
}

void UAWeekGameUIManager::DeactivateChestInventory()
{
	PlayerCharacter->SetChestInventoryComponent(nullptr);
	
	if (InventoryController->IsHoldingItem())
	{
		if (InventoryController->GetHeldItem()->GetSourceInventory() == InventoryMainPanelWidget->GetChestInventoryComponent())
		{
			InventoryController->ReturnHeldItemToInventory();
		}
	}
	InventoryMainPanelWidget->DeActivateChestInventory();
}