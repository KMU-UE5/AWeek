// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerState.h"
#include "../UI/Player/AWeekStaminaWidget.h"
#include "../UI/AWeekUIManager.h"

DEFINE_LOG_CATEGORY(AWeekState);

AAWeekPlayerState::AAWeekPlayerState(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAWeekPlayerState::BeginPlay()
{
	Super::BeginPlay();

	mStaminaWidget = CAWeekUIManager::GetInst()->FindWidget<UAWeekStaminaWidget>(TEXT("StaminaWidget"));
}

void AAWeekPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStaminaRecovery && mStamina < mMaxStamina)
	{
		mStamina = FMath::Min(mStamina + mStaminaRecoveryRate * DeltaTime, mMaxStamina);
		mStaminaWidget->SetProgress(mStamina);
	}
	
	UE_LOG(AWeekState, Warning, TEXT("%f"), mStamina);
}

bool AAWeekPlayerState::UseStamina(float Usage)
{
	if (mStamina < Usage)
		return false;

	mStamina -= Usage;
	mStaminaWidget->SetProgress(mStamina);
	mStaminaWidget->SetVisibility(ESlateVisibility::Visible);
	bStaminaRecovery = false;

	GetWorldTimerManager().ClearTimer(mStaminaRecoveryTimer);
	GetWorldTimerManager().SetTimer(
		mStaminaRecoveryTimer,
		this,
		&AAWeekPlayerState::EnableStaminaRecovery,
		mStaminaRecoveryCool,
		false
	);

	GetWorldTimerManager().ClearTimer(mStaminaAnimationTimer);
	GetWorldTimerManager().SetTimer(
		mStaminaAnimationTimer,
		this,
		&AAWeekPlayerState::PlayDisappearAnim,
		mStaminaAnimationCool,
		false
	);

	return true;
}

void AAWeekPlayerState::PlayDisappearAnim()
{
	if (mStaminaWidget)
	{
		mStaminaWidget->PlayDisappearAnimation();
	}
}
