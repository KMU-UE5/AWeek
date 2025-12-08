#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnVolumeActor.generated.h"

UCLASS()
class AWEEK_API ASpawnVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolumeActor();
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Spawn")
	UBoxComponent* VolumeBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	bool bIsDenyVolume = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	float SpawnWeight = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	bool IsLocationInside(const FVector& Transform) const;

#if WITH_EDITOR
	void UpdateWireframeColor();
	virtual void OnConstruction(const FTransform& WorldLocation) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
