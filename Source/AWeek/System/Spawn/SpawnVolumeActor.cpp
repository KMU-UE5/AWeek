#include "SpawnVolumeActor.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

ASpawnVolumeActor::ASpawnVolumeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent->SetWorldLocation(FVector::ZeroVector); // Pivot at floor

	VolumeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	VolumeBox->SetupAttachment(RootComponent);
	VolumeBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VolumeBox->SetGenerateOverlapEvents(false);
	VolumeBox->bHiddenInGame = true;

	// default Values
	// Set relative location so bottom of box aligns with pivot/floor
	VolumeBox->SetRelativeLocation(FVector(0.f, 0.f, VolumeBox->GetScaledBoxExtent().Z));
	VolumeBox->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	bIsDenyVolume = false;
	SpawnWeight = 1.0f;
}

bool ASpawnVolumeActor::IsLocationInside(const FVector& WorldLocation) const
{
	if (!VolumeBox)
		return false;
	const FTransform BoxTransform = VolumeBox->GetComponentTransform();
	const FVector ScaledExtents = VolumeBox->GetScaledBoxExtent();
	return UKismetMathLibrary::IsPointInBoxWithTransform(WorldLocation, BoxTransform, ScaledExtents);
}

#if WITH_EDITOR
void ASpawnVolumeActor::UpdateWireframeColor()
{
	if (VolumeBox)
	{
		// Deny면 빨강, 아니면 초록
		VolumeBox->ShapeColor = bIsDenyVolume ? FColor::Red : FColor::Green;
		VolumeBox->MarkRenderStateDirty();

		// 바닥 Pivot 유지
		const FVector Extents = VolumeBox->GetScaledBoxExtent();
		VolumeBox->SetRelativeLocation(FVector(0.f, 0.f, Extents.Z));
	}
}

void ASpawnVolumeActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdateWireframeColor();
}

void ASpawnVolumeActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASpawnVolumeActor, bIsDenyVolume) ||
		PropertyName == FName("BoxExtent"))
	{
		UpdateWireframeColor();
	}
}
#endif

void ASpawnVolumeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnVolumeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

