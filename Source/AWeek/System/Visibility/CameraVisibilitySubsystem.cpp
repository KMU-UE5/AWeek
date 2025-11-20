#include "CameraVisibilitySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

void UCameraVisibilitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

bool UCameraVisibilitySubsystem::IsLocationVisible(const FVector& WorldLocation, bool bCheckOcclusion, bool bDebug) const
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return false;
    //Is On the Screen
    if (!IsLocationOnScreen(WorldLocation))
    {
        if (bDebug)
        {
            UE_LOG(LogTemp, Verbose, TEXT("[CameraVisibility] Location OFFSCREEN: %s"), *WorldLocation.ToString());
        }
        return false;
    }

    //Check Occlusion
    if (bCheckOcclusion)
    {
        FVector ViewLoc;
        FRotator ViewRot;
        PC->GetPlayerViewPoint(ViewLoc, ViewRot);

        bool bVisible = OcclusionCheck(ViewLoc, WorldLocation, bDebug);
        if (bDebug)
        {
            UE_LOG(LogTemp, Verbose, TEXT("[CameraVisibility] OcclusionCheck for %s => %s (Cam=%s)"),
                *WorldLocation.ToString(),
                bVisible ? TEXT("VISIBLE") : TEXT("BLOCKED"),
                *ViewLoc.ToString());
        }
        return bVisible;
    }

    return true;
}

bool UCameraVisibilitySubsystem::IsLocationOnScreen(const FVector& WorldLocation) const
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PC) return false;

    FVector2D ScreenPos;

    PC->ProjectWorldLocationToScreen(WorldLocation, ScreenPos);

    int32 SizeX = 0, SizeY = 0;
    PC->GetViewportSize(SizeX, SizeY);

    return (ScreenPos.X >= 0.f && ScreenPos.X <= SizeX &&
        ScreenPos.Y >= 0.f && ScreenPos.Y <= SizeY);
}

bool UCameraVisibilitySubsystem::OcclusionCheck(const FVector& CameraLocation, const FVector& TargetLocation, bool bDebug) const
{
    UWorld* World = GetWorld();
    if (!World) return false;

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(CameraVisibilityTrace), true);
    Params.bReturnPhysicalMaterial = false;

    APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
    if (PC && PC->GetViewTarget())
    {
        Params.AddIgnoredActor(PC->GetViewTarget());
    }

    // LineTrace
    bool bHit = World->LineTraceSingleByChannel(Hit,CameraLocation,TargetLocation,ECC_Visibility,Params);

    // Debug: draw the trace line
    if (bDebug)
    {
        const FColor LineColor = bHit ? FColor::Red : FColor::Green;
        DrawDebugLine(World, CameraLocation, TargetLocation, LineColor, false, 1.5f, 0, 1.5f);

        if (bHit)
        {
            DrawDebugSphere(World, Hit.ImpactPoint, 6.f, 8, FColor::Red, false, 2.0f);
            DrawDebugString(World, Hit.ImpactPoint + FVector(0, 0, 12.f), FString::Printf(TEXT("Hit: %s"), *GetNameSafe(Hit.GetActor())), nullptr, FColor::White, 2.f);
        }

        DrawDebugSphere(World, TargetLocation, 6.f, 8, FColor::Blue, false, 2.0f);
        DrawDebugString(World, TargetLocation + FVector(0, 0, 12.f), TEXT("Target"), nullptr, FColor::White, 2.f);
    }

    //LineTrace Check
    if (!bHit)
        return true;

    const float DistCamToHit = FVector::Dist(CameraLocation, Hit.ImpactPoint);
    const float DistCamToTarget = FVector::Dist(CameraLocation, TargetLocation);

    const float Epsilon = 1.0f; 

    bool bVisibleByDist = (DistCamToHit >= DistCamToTarget - Epsilon);

    if (bDebug)
    {
        UE_LOG(LogTemp, Verbose, TEXT("[CameraVisibility] DistCamToHit=%.3f DistCamToTarget=%.3f visibleByDist=%d"), DistCamToHit, DistCamToTarget, bVisibleByDist);
    }

    return bVisibleByDist;
}
