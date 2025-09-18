// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AWeekDaySystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayChanged, bool, IsDawn);

UCLASS()
class AWEEK_API AAWeekDaySystem : public AActor
{
	GENERATED_BODY()

public:
    AAWeekDaySystem();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    bool Dawn = false;
    bool Midnight = false;

    // 하루 길이 (초 단위, 예: 300초 = 5분)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    float DayLengthInSeconds = 300.f;

    // 현재 시간 (0 ~ 24)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    float TimeOfDay = 6;

    // 빛 세기
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    float Intensity = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    ADirectionalLight* SunLight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    ASkyLight* SkyLightActor;

private:
    void UpdateLighting();

public:
    UPROPERTY(BlueprintAssignable, Category = "DaySystem")
    FOnDayChanged OnDayChanged;
};