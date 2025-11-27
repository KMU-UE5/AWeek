// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeek/Grid/GridPlacedActor.h"
#include "FlamethrowerGridPlacedActor.generated.h"

/**
 * 
 */
class USceneComponent;
class USphereComponent;
class UNiagaraComponent;
class UNiagaraSystem;
UCLASS()
class AWEEK_API AFlamethrowerGridPlacedActor : public AGridPlacedActor
{
	GENERATED_BODY()
public:
	AFlamethrowerGridPlacedActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/*UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;*/

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TurretHead;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* YawPivot;

	UPROPERTY(EditAnywhere)
	USphereComponent* DetectCapsule;

	UPROPERTY(EditAnywhere, Category="Turret")
	TSubclassOf<AActor> TargetClass;
	//GunRange
	UPROPERTY(EditAnywhere, Category="Turret|Trace")
	float TraceDistance = 1500.f;
	//LineTrace channel 
	UPROPERTY(EditAnywhere, Category="Turret|Trace")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	
	FTimerHandle TraceTimerHandle;

	UPROPERTY(EditAnywhere, Category="Turret")
	float RotateSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category="Turret|Trace")
	float TraceInterval = 0.5f;

	UFUNCTION()
	void TurretLineTrace();

	UPROPERTY(EditAnywhere, Category="Turret")
	bool bYawOnly = true;


	//FlameFX
	UPROPERTY(EditAnywhere)
	UNiagaraComponent* FlameFX;
	UPROPERTY(EditAnywhere,Category="Flame")
	UNiagaraSystem* FlameFXSystem;
	UPROPERTY(EditAnywhere, Category="Flame")
	float MaxRange = 1000.f;
	UPROPERTY(EditAnywhere)
	UBoxComponent* DamageBox;

	UPROPERTY(EditAnywhere, Category="Flame")
	bool bIsFiring = false;


	void UpdateFlame(float DeltaTime);
	float DoFlameTrace()const;
	void UpdateDamageVolume(float EffectiveLength);

	UFUNCTION(BlueprintCallable)
	void StartFlame();
	UFUNCTION(BlueprintCallable)
	void EndFlame();
	
};
