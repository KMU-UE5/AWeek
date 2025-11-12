// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem.h"

void USettingItem::Init()
{
}

void USettingItem::NotifySettingChanged(const ESettingChangedReason Reason)
{
	OnSettingChangedEvent.Broadcast(this, Reason);
}

void USettingItem::Apply()
{
	OnSettingAppliedEvent.Broadcast(this);
}
