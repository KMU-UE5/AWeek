// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/World/AWeekChest.h"
#include "AWeekLootChest.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API AAWeekLootChest : public AAWeekChest
{
	GENERATED_BODY()
	
public:
	static AAWeekLootChest* SpawnLootChest(
		const TSubclassOf<AAWeekChest> LootChestClass,
		UWorld* World,
		const FVector& Location,
		FDataTableRowHandle LootTableHandle,
		float LifeSpan = 30.0f
	);
	
	void AWeekLootChest();

	void InitializeLootChest(const FDataTableRowHandle& InLootTableHandle, int32 InLifeSpan);
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere)
	float LifeSpan;

	UPROPERTY()
	FTimerHandle LifeSpanTimer;

	UPROPERTY(EditAnywhere, Category = "Loot")
	FDataTableRowHandle LootTableRowHandle;

	UPROPERTY(EditAnywhere, Category = "Loot")
	bool bTimeLimit;
	
	UFUNCTION()
	void DestoryOnTimerExpired();

	TArray<FAWeekItemEntry> GenerateRandomLoot();
};
