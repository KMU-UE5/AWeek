// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekCrossHairWidget.h"

#include "AWeek/Data/AWeekWeaponInfo.h"

float UAWeekCrossHairWidget::GetFinalSpreadAngle() const
{
	EWeaponType WeaponType = EWeaponType::Ranged;
	if (WeaponType == EWeaponType::Ranged)
	{
		
	}

	return 0.0f;
}

float UAWeekCrossHairWidget::GetScreenspaceMaxSpreadAngleRadius() const
{
	return 0.0f;
}

bool UAWeekCrossHairWidget::HasFirstShotAccuracy() const
{
	EWeaponType WeaponType = EWeaponType::Ranged;
	if (WeaponType == EWeaponType::Ranged)
	{
		return true;
	}

	return false;
}
