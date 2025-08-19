// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AWeekPlayerState.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(AWeekState, Warning, All);

UCLASS()
class AWEEK_API AAWeekPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AAWeekPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(EditAnywhere)
	float mMaxStamina = 100;

	UPROPERTY(VisibleAnywhere)
	float mStamina = 100;

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

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


public:
	bool UseStamina(float Usage);
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


