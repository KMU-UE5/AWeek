// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AWeekStaminaComponent.generated.h"

UENUM()
enum class EStaminaUseType
{
	Sprint,
	Vault,
	Ledge,
	LedgeStart
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AWEEK_API UAWeekStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAWeekStaminaComponent();

protected:
protected:
	UPROPERTY(EditAnywhere)
	float mMaxStamina = 100; // 원래값 100

	UPROPERTY(VisibleAnywhere)
	float mStamina = 100; // 원래값 100

	UPROPERTY(EditAnywhere)
	float mStaminaRecoveryRate = 20;

	UPROPERTY(EditAnywhere)
	float mStaminaRecoveryCool = 2;

	UPROPERTY(EditAnywhere)
	float mStaminaAnimationCool = mStaminaAnimationCool + 2;

	FTimerHandle mStaminaRecoveryTimer;
	FTimerHandle mStaminaAnimationTimer;
	bool bStaminaRecovery = true;

	class UAWeekStaminaWidget* mStaminaWidget;

protected:
	UPROPERTY(EditAnywhere)
	float mSprintUsage = 20; // 초당 소모량

	UPROPERTY(EditAnywhere)
	float mVaultUsage = 10;

	UPROPERTY(EditAnywhere)
	float mLedgeUsage = 10; // 초당 소모량

	UPROPERTY(EditAnywhere)
	float mLedgeStartUsage = 10;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool UseStamina(EStaminaUseType StaminaUseType);
	float GetStamina()
	{
		return mStamina;
	}
	void EnableStaminaRecovery()
	{
		bStaminaRecovery = true;
	}
	void PlayDisappearAnim();
};
