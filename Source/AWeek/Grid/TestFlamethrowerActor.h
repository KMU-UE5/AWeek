// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestFlamethrowerActor.generated.h"


class UBoxComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class AWEEK_API ATestFlamethrowerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestFlamethrowerActor();

	UFUNCTION(BlueprintCallable)
	void StartFlame();
	UFUNCTION(BlueprintCallable)
	void EndFlame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* Muzzle;
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* FlameFX;
	UPROPERTY(EditAnywhere,Category="Flame")
	UNiagaraSystem* FlameFXSystem;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* DamageBox;

	UPROPERTY(EditAnywhere, Category="Flame")
	float MaxRange = 100.f;
	UPROPERTY(EditAnywhere, Category="Flame")
	float DamagePerSecond = 20.f;

	UPROPERTY(EditAnywhere, Category="Flame")
	bool bIsFiring = false;


	void UpdateFlame(float DeltaTime);
	float DoFlameTrace()const;
	void UpdateDamageVolume(float EffectiveLength);
	void ApplyFlameDamage();
	void UpdateNiagaraFX(float EffectiveLength);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
