// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameInstance.h"

#include "AWeek/AWeekGameplayTags.h"

void UAWeekGameInstance::Init()
{
	Super::Init();
	FAWeekGameplayTags::InitializeNativeTags();
}

void UAWeekGameInstance::Shutdown()
{
	Super::Shutdown();
}
