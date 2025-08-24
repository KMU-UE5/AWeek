// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameplayTags.h"
#include "GameplayTagsManager.h"

FAWeekGameplayTags FAWeekGameplayTags::GameplayTags;
// 에디터가 켜지기도 전 실행
void FAWeekGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	GameplayTags.AddAllTags(Manager);
}

void FAWeekGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ") + FString(TagComment)));
}

void FAWeekGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(UI_Layer_Game, "UI.Layer.Game", "UI Layer Game");
	AddTag(UI_Layer_GameMenu, "UI.Layer.GameMenu", "UI Layer GameMenu");
	AddTag(UI_Layer_Menu, "UI.Layer.Menu", "UI Layer Menu");
	AddTag(UI_Layer_Modal, "UI.Layer.Modal", "UI Layer Modal");
}
