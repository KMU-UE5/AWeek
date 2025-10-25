// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeaponInfo.h"

float FRangedWeaponInfo::ClampHeatRange(float HeatValue)
{
	float MinTime, MaxTime;
	
	ComputeHeatRange(HeatShotCurve, MinTime, MaxTime);
	float MinHeat = MinTime;
	float MaxHeat = MaxTime;
	
	ComputeHeatRange(HeatShotRecoveryCurve, MinTime, MaxTime);
	MinHeat = FMath::Min(MinHeat, MinTime);
	MaxHeat = FMath::Max(MaxHeat, MinTime);

	ComputeHeatRange(HeatToSpreadCurve, MinTime, MaxTime);
	MinHeat = FMath::Min(MinHeat, MinTime);
	MaxHeat = FMath::Max(MaxHeat, MaxTime);

	UE_LOG(LogTemp, Warning, TEXT("Min: %f, Max: %f"), MinHeat, MaxHeat);
	return FMath::Clamp(HeatValue, MinHeat, MaxHeat);
}

void FRangedWeaponInfo::ComputeHeatRange(const FRuntimeFloatCurve& RuntimeFloatCurve, float& MinTime, float& MaxTime)
{
	RuntimeFloatCurve.GetRichCurveConst()->GetTimeRange(MinTime, MaxTime);
}
