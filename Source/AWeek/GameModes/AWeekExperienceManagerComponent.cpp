// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekExperienceManagerComponent.h"

#include "AWeekExperienceDefinition.h"
#include "Engine/AssetManager.h"

void UAWeekExperienceManagerComponent::ServerSetCurrentExperience(const FPrimaryAssetId& ExperienceId)
{
	const UAssetManager& AssetManager = UAssetManager::Get();

	const FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
	TSubclassOf<UAWeekExperienceDefinition> AssetClass = Cast<UClass>(AssetPath.TryLoad());

	const UAWeekExperienceDefinition* Experience = GetDefault<UAWeekExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();
}

void UAWeekExperienceManagerComponent::StartExperienceLoad()
{
	
}
