// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekDaySystem.h"

AAWeekDaySystem::AAWeekDaySystem()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AAWeekDaySystem::BeginPlay()
{
    Super::BeginPlay();
}

void AAWeekDaySystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 하루 진행도 (0~1)
    // 6: Dawn / 12: Midnight
    float DayProgress = (DeltaTime / DayLengthInSeconds) * 24.f;
    TimeOfDay += DayProgress;
    if (TimeOfDay >= 24.f) TimeOfDay -= 24.f;

    UpdateLighting();
}

void AAWeekDaySystem::UpdateLighting()
{
    if (!Dawn && FMath::IsNearlyEqual(TimeOfDay, 6.0f, 0.01f))
    {
        Dawn = true;
        Midnight = false;
        OnDayChanged.Broadcast(Dawn);
    }

    if (!Midnight && FMath::IsNearlyEqual(TimeOfDay, 24.f, 0.01f))
    {
        Dawn = false;
        Midnight = true;
        OnDayChanged.Broadcast(Dawn);
    }

    if (SunLight)
    {
        // Rotate Directional Light
        float Pitch = (TimeOfDay / 24.f) * 360.f + 90.f;
        FRotator SunRotation = FRotator(Pitch, -90.f, 0.f);
        SunLight->SetActorRotation(SunRotation);

        if (SunLight->GetLightComponent())
        {
            SunLight->GetLightComponent()->SetIntensity(Intensity);
        }
    }

    if (SkyLightActor && SkyLightActor->GetLightComponent())
    {
        SkyLightActor->GetLightComponent()->RecaptureSky();
    }
}