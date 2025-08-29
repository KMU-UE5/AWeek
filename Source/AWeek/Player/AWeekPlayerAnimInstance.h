// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Character/AWeekCharacter.h"
#include "../Data/AWeekPlayerAnimInfo.h"
#include "Animation/AnimInstance.h"
#include "AWeekPlayerAnimInstance.generated.h"

UENUM(BlueprintType)
enum class EPlayerMoveState : uint8
{
	Ground,
	Ledge,
	Climb
};

UCLASS()
class AWEEK_API UAWeekPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AAWeekCharacter> mOwner;

	UPROPERTY(EditAnywhere)
	FName mStatusKey = TEXT("Default");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, FPlayerAnimInfo> mAnimMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerMoveState mMoveState = EPlayerMoveState::Ground;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimSequence>>	mSequenceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UBlendSpace>>	mBlendSpaceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>>	mMontageMap;

	UAnimMontage* mRunToStopMontage;
	UAnimMontage* mOneHandVaultMontage;
	UAnimMontage* mLedgeMontage;
	UAnimMontage* mClimbMontage;

public:
	virtual void NativeBeginPlay();
	virtual void NativeInitializeAnimation();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	UFUNCTION(BlueprintCallable)
	UAnimSequence* FindAnimSequence(const FName& Name);

	UFUNCTION(BlueprintCallable)
	UBlendSpace* FindBlendSpace(const FName& Name);

	UFUNCTION(BlueprintCallable)
	UAnimMontage* FindAnimMontage(const FName& Name);

public:
	void ChangeAnimOverride(FName State)
	{
		mStatusKey = State;

		mSequenceMap = mAnimMap[mStatusKey].SequenceMap;
		mBlendSpaceMap = mAnimMap[mStatusKey].BlendSpaceMap;
		mMontageMap = mAnimMap[mStatusKey].MontageMap;

		mRunToStopMontage = FindAnimMontage(TEXT("RunToStop"));
		mOneHandVaultMontage = FindAnimMontage(TEXT("OneHandVault"));
		mLedgeMontage = FindAnimMontage(TEXT("Ledge"));
		mClimbMontage = FindAnimMontage(TEXT("Climb"));
	}

	FName GetCurrentOverride()
	{
		return mStatusKey;
	}

	EPlayerMoveState GetPlayerMoveState()
	{
		return mMoveState;
	}
	void SetPlayerMoveState(EPlayerMoveState MoveState)
	{
		mMoveState = MoveState;
	}

	void PlayRunToStopMontage()
	{
		Montage_Play(mRunToStopMontage);
	}

	bool IsPlayingRunToStopMontage()
	{
		return Montage_IsPlaying(mRunToStopMontage);
	}

	void PlayVaultMontage()
	{
		Montage_Play(mOneHandVaultMontage);
	}

	void PlayLedgeMontage()
	{
		Montage_Play(mLedgeMontage);
	}

	bool IsPlayingLedgeMontage()
	{
		return Montage_IsPlaying(mLedgeMontage);
	}

	void PlayClimbMontage()
	{
		Montage_Play(mClimbMontage);
	}

	bool IsPlayingClimbMontage()
	{
		return Montage_IsPlaying(mClimbMontage);
	}

protected:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);
};
