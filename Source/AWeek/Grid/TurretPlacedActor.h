// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/GridPlacedActor.h"
#include "TurretPlacedActor.generated.h"

/**
 * 
 */
class UTurretComponent;
class USphereComponent;
class UArrowComponent;
UCLASS()
class AWEEK_API ATurretPlacedActor : public AGridPlacedActor
{
	GENERATED_BODY()
public:
	ATurretPlacedActor();

	protected:
	UPROPERTY(EditAnywhere, Category="Turret")
	USceneComponent* YawPivot;
	UPROPERTY(EditAnywhere, Category="Turret")
	USceneComponent* PitchPivot;

	UPROPERTY(EditAnywhere, Category="Turret")
	UStaticMeshComponent* TurretHead;


	UPROPERTY(EditAnywhere, Category="Turret")
	UArrowComponent* MuzzleArrow;
	
	/*UPROPERTY(EditAnywhere, Category="Turret")
	USceneComponent* Muzzle;
	*/

	UPROPERTY(VisibleAnywhere, Category="Turret")
	UTurretComponent* TurretLogic;

	UPROPERTY(EditAnywhere, Category="Turret")
	USphereComponent* SenseSphere;

	UFUNCTION()
	void OnSenseBegin(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp,
					  int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep);

	UFUNCTION()
	void OnSenseEnd(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex);

protected:
	virtual void BeginPlay() override;	

	
};
