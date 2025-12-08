#include "AWeekActorSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "NavigationPath.h"

AAWeekActorSpawner::AAWeekActorSpawner()
{
    PrimaryActorTick.bCanEverTick = false;

    bUseVolumeWeight = true;
    bSpawnActorRandomRotation = true;
    bIsPathFinding = true;
    PathFindingTarget = nullptr;
}

void AAWeekActorSpawner::BeginPlay()
{
    Super::BeginPlay();
}

const ASpawnVolumeActor* AAWeekActorSpawner::ChooseRandomAllowVolume() const
{
    TArray<ASpawnVolumeActor*> AllowVolumes = Volumes.FilterByPredicate([](ASpawnVolumeActor* V) { return V && !V->bIsDenyVolume; });
    if (AllowVolumes.Num() == 0) return nullptr;

    if (!bUseVolumeWeight)
        return AllowVolumes[FMath::RandRange(0, AllowVolumes.Num() - 1)];

    float TotalWeight = 0.f;
    for (ASpawnVolumeActor* Vol : AllowVolumes)
        TotalWeight += Vol->SpawnWeight;

    float Rand = FMath::FRandRange(0.f, TotalWeight);
    float Accum = 0.f;

    for (ASpawnVolumeActor* Vol : AllowVolumes)
    {
        Accum += Vol->SpawnWeight;
        if (Rand <= Accum)
            return Vol;
    }

    return AllowVolumes[0];
}

FVector AAWeekActorSpawner::GetRandomLocationInVolumes() const
{
    if (Volumes.Num() == 0) return FVector::ZeroVector;

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSys) return FVector::ZeroVector;

    FVector RandomPoint = FVector::ZeroVector;
    int32 MaxAttempts = 50;

    for (int32 i = 0; i < MaxAttempts; i++)
    {
        const ASpawnVolumeActor* Volume = ChooseRandomAllowVolume();
        if (!Volume) continue;

        const FVector Origin = Volume->VolumeBox->GetComponentLocation();
        const FVector Extent = Volume->VolumeBox->GetScaledBoxExtent();

        FNavLocation NavLocation;
        bool bFound = NavSys->GetRandomPointInNavigableRadius(Origin, Extent.GetMax(), NavLocation);
        if (!bFound) continue;

        // Box Check
        FVector Local = NavLocation.Location - Origin;
        if (FMath::Abs(Local.X) > Extent.X || FMath::Abs(Local.Y) > Extent.Y || FMath::Abs(Local.Z) > Extent.Z)
            continue;

        // PathFinding Check
        if (bIsPathFinding && PathFindingTarget)
        {
            UNavigationPath* Path = NavSys->FindPathToActorSynchronously(GetWorld(), NavLocation.Location, PathFindingTarget);
            if (!Path || Path->PathPoints.Num() == 0)
                continue;

            // NavLocation Check
            const FNavLocation StartNav(NavLocation.Location);
            const FNavLocation EndNav(PathFindingTarget->GetActorLocation());
            if (!Path || Path->PathPoints.Num() == 0)
                continue;
        }

        RandomPoint = NavLocation.Location;
        break;
    }

    if (RandomPoint.IsZero())
        UE_LOG(LogTemp, Warning, TEXT("Spawner: Failed to find valid spawn location!"));

    return RandomPoint;
}

void AAWeekActorSpawner::SpawnActor()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (const FSpawnInfo& Info : SpawnList)
    {
        if (!Info.ActorClass) continue;

        for (int32 i = 0; i < Info.SpawnCount; i++)
        {
            FVector SpawnLocation = GetRandomLocationInVolumes();
            if (SpawnLocation.IsZero()) continue;

            float ZOffset = 0.f;
            if (ACharacter* DefaultChar = Cast<ACharacter>(Info.ActorClass->GetDefaultObject()))
            {
                if (UCapsuleComponent* Capsule = DefaultChar->GetCapsuleComponent())
                    ZOffset = Capsule->GetScaledCapsuleHalfHeight();
            }

            SpawnLocation.Z += ZOffset;

            FRotator SpawnRot = FRotator::ZeroRotator;
            if (bSpawnActorRandomRotation)
                SpawnRot = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            AActor* SpawnedActor = World->SpawnActor<AActor>(Info.ActorClass, SpawnLocation, SpawnRot, SpawnParams);

#if WITH_EDITOR
            if (SpawnedActor)
                EditorSpawnedActors.Add(SpawnedActor);
#endif
        }
    }
}

#if WITH_EDITOR
void AAWeekActorSpawner::ClearTestSpawnedActors()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (AActor* Actor : EditorSpawnedActors)
    {
        if (Actor && !Actor->IsActorBeingDestroyed())
            Actor->Destroy();
    }
    EditorSpawnedActors.Empty();
}
#endif
