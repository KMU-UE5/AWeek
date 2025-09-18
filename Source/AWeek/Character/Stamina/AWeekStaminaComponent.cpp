// Fill out your copyright notice in the Description page of Project Settings.

#include "AWeekStaminaComponent.h"
#include "../../UI/AWeekUIManager.h"
#include "../../UI/Player/AWeekStaminaWidget.h"

UAWeekStaminaComponent::UAWeekStaminaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAWeekStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	mStaminaWidget = CAWeekUIManager::GetInst()->FindWidget<UAWeekStaminaWidget>(TEXT("StaminaWidget"));
	
}

void UAWeekStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bStaminaRecovery && mStamina < mMaxStamina)
	{
		mStamina = FMath::Min(mStamina + mStaminaRecoveryRate * DeltaTime, mMaxStamina);
		mStaminaWidget->UpdateProgress(mStamina);
	}
}

bool UAWeekStaminaComponent::UseStamina(EStaminaUseType StaminaUseType)
{
	float Usage = 0;
	float DeltaTime = GetWorld()->DeltaTimeSeconds;

	switch (StaminaUseType)
	{
	case EStaminaUseType::Sprint:
		Usage = mSprintUsage * DeltaTime;
		break;
	case EStaminaUseType::Vault:
		Usage = mVaultUsage;
		break;
	case EStaminaUseType::Ledge:
		Usage = mLedgeUsage * DeltaTime;
		break;
	case EStaminaUseType::LedgeStart:
		Usage = mLedgeStartUsage;
		break;
	}

	if (mStamina < Usage)
		return false;

	mStamina -= Usage;
	mStaminaWidget->AfterUseStamina(mStamina);
	bStaminaRecovery = false;

	GetOwner()->GetWorldTimerManager().ClearTimer(mStaminaRecoveryTimer);
	GetOwner()->GetWorldTimerManager().SetTimer(
		mStaminaRecoveryTimer,
		this,
		&UAWeekStaminaComponent::EnableStaminaRecovery,
		mStaminaRecoveryCool,
		false
	);

	GetOwner()->GetWorldTimerManager().ClearTimer(mStaminaAnimationTimer);
	GetOwner()->GetWorldTimerManager().SetTimer(
		mStaminaAnimationTimer,
		this,
		&UAWeekStaminaComponent::PlayDisappearAnim,
		mStaminaAnimationCool,
		false
	);

	return true;
}

void UAWeekStaminaComponent::PlayDisappearAnim()
{
	if (mStaminaWidget)
	{
		mStaminaWidget->PlayDisappearAnimation();
	}
}

