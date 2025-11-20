#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraVisibilitySubsystem.generated.h"

UCLASS()
class AWEEK_API UCameraVisibilitySubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "Camera|Visibility")
    bool IsLocationVisible(const FVector& WorldLocation, bool bCheckOcclusion = true, bool bDebug = false) const;

    UFUNCTION(BlueprintCallable, Category = "Camera|Visibility")
    bool IsLocationOnScreen(const FVector& WorldLocation) const;

private:

    bool OcclusionCheck(const FVector& CameraLocation, const FVector& TargetLocation, bool bDebug = false) const;
};
