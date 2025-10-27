// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem.h"

void USettingItem::NotifySettingChanged()
{
	OnSettingChangedEvent.Broadcast(this);
}

void USettingItem::Apply()
{
	OnSettingAppliedEvent.Broadcast(this);
}
