// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/World/AWeekLootChest.h"
#include "AWeek/Components/AWeekInventoryComponent.h"
#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "AWeek/Data/AWeekItemDataStructs.h"
#include "AWeek/Data/AWeekLootItemData.h"
#include "AWeek/Items/AWeekItemBase.h"


AAWeekLootChest* AAWeekLootChest::SpawnLootChest(const TSubclassOf<AAWeekChest> LootChestClass,
	UWorld* World, const FVector& Location,
	FDataTableRowHandle LootTableHandle, float LifeSpan)
{
	if (!World)
	{
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAWeekLootChest* LootChest = World->SpawnActor<AAWeekLootChest>(
		LootChestClass,
		Location,
		FRotator::ZeroRotator,
		SpawnParams
		);

	if (LootChest)
	{
		LootChest->InitializeLootChest(LootTableHandle, LifeSpan);
	}

	return LootChest;
}

void AAWeekLootChest::AWeekLootChest()
{
	bTimeLimit = false;
	LifeSpan = 20.0f;

	ChestMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChestMesh->SetSimulatePhysics(false);
}

void AAWeekLootChest::InitializeLootChest(const FDataTableRowHandle& InLootTableHandle, int32 InLifeSpan)
{	
	LootTableRowHandle = InLootTableHandle;
	LifeSpan = InLifeSpan;

	if (bTimeLimit)
	{
		GetWorldTimerManager().SetTimer(LifeSpanTimer,
		this,
		&AAWeekLootChest::DestoryOnTimerExpired,
		LifeSpan,
		false);	
	}

	if (InventoryComponent)
	{
		TArray<FAWeekItemEntry> Items = GenerateRandomLoot();
		for (const FAWeekItemEntry& ItemEntry : Items)
		{
			UAWeekItemBase* NewItem = UAWeekItemBase::CreateFromData(
				ItemEntry.ItemData,
				ItemEntry.Quantity,
				InventoryComponent
			);

			if (NewItem)
			{
				FAWeekItemAddResult Result = InventoryComponent->HandleAddItem(NewItem);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("New Item is null!"));
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent is null!"));
	}
}

void AAWeekLootChest::BeginPlay()
{
	Super::BeginPlay();

	InitializeLootChest(LootTableRowHandle, LifeSpan);
}

void AAWeekLootChest::DestoryOnTimerExpired()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->CloseChestInventory();
		PlayerCharacter = nullptr;
	}
	Destroy();
}

TArray<FAWeekItemEntry> AAWeekLootChest::GenerateRandomLoot()
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(__FUNCTION__));
	
	if (LootTableRowHandle.IsNull())
	{
		UE_LOG(LogTemp, Log, TEXT("%s: row handle is null"), *FString(__FUNCTION__));
		return TArray<FAWeekItemEntry>();
	}
    
	const FAWeekLootItemData* LootDropItemData = LootTableRowHandle.GetRow<FAWeekLootItemData>(LootTableRowHandle.RowName.ToString());
	if (!LootDropItemData)
	{
		UE_LOG(LogTemp, Log, TEXT("%s: LootDropItemData is null"), *FString(__FUNCTION__));
		return TArray<FAWeekItemEntry>();
	}
    
	TArray<FAWeekItemEntry> GeneratedItems;
    
	int32 NumItemsToDrop = FMath::RandRange(LootDropItemData->MinItemsToDrop, LootDropItemData->MaxItemsToDrop);

	for (const FAWeekLootItemEntry& Entry : LootDropItemData->PossibleLootItemEntries)
	{
		if (GeneratedItems.Num() >= NumItemsToDrop)
		{
			break;
		}
        
		float RandomChance = FMath::FRandRange(0.0f, 100.0f);
		if (RandomChance > Entry.DropChance)
		{
			continue;
		}
        
		const FAWeekItemData* ItemData = Entry.ItemHandle.GetRow<FAWeekItemData>(Entry.ItemHandle.RowName.ToString());
		if (!ItemData)
		{
			UE_LOG(LogTemp, Log, TEXT("%s: ItemData is null"), *FString(__FUNCTION__));
			continue;
		}
        
		int32 Quantity = FMath::RandRange(Entry.MinQuantity, Entry.MaxQuantity);

		GeneratedItems.Add(FAWeekItemEntry(*ItemData, Quantity));
	}
	
	return GeneratedItems;
}
