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

USTRUCT(BlueprintType)
struct FDayChangedMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    bool bIsDay = true;
};

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
    bool bDayChangeFlag = false;
    // 하루 길이 (초 단위, 예: 300초 = 5분)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    float DayLengthInSeconds = 300.f;

    // 현재 시간 (0 ~ 24)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DaySystem")
    float TimeOfDay = 6.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    ADirectionalLight* Sun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DaySystem")
    float SunIntensity = 4.f;

private:
    void UpdateLighting();
};