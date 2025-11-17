// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Building/BaseBuildingWheelPanel.h"

#include "BuildingSelectWidget.h"

void UBaseBuildingWheelPanel::ActiveWheel()
{
	if (Index == SectionCount)Index = 0;
	UE_LOG(LogTemp, Log, TEXT("Base Building select widget click action %d"), Index);
	BuildingSelectWidget->ChangeWheel(Index+1);
}
