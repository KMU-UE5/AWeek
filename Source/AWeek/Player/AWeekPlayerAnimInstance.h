// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

UENUM(BlueprintType)
enum class EPlayerWeaponState : uint8
{
	Default,
	Gun
};

UCLASS()
class AWEEK_API UAWeekPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<class AAWeekPlayerCharacter> mOwner;

	UPROPERTY(EditAnywhere)
	FName mStatusKey = TEXT("Default");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, FPlayerAnimInfo> mAnimMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerMoveState mMoveState = EPlayerMoveState::Ground;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerWeaponState mWeaponState = EPlayerWeaponState::Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimSequence>>	mSequenceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UBlendSpace>>	mBlendSpaceMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FName, TObjectPtr<UAnimMontage>>	mMontageMap;

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
		mWeaponState = EPlayerWeaponState::Default;

		mSequenceMap = mAnimMap[mStatusKey].SequenceMap;
		mBlendSpaceMap = mAnimMap[mStatusKey].BlendSpaceMap;
		mMontageMap = mAnimMap[mStatusKey].MontageMap;
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

	EPlayerWeaponState GetPlayerWeaponState()
	{
		return mWeaponState;
	}

	void SetPlayerWeaponState(EPlayerWeaponState WeaponState)
	{
		mWeaponState = WeaponState;
	}

	void PlayMontageByName(FName Name, float PlayRate = 1.0f)
	{
		UAnimMontage* Montage = FindAnimMontage(Name);
		if (Montage)
		{
			Montage_Play(Montage, PlayRate);
		}
	}

	void StopMontageByName(FName Name, float BlendRate = 1.0f)
	{
		UAnimMontage* Montage = FindAnimMontage(Name);
		if (Montage)
		{
			Montage_Stop(BlendRate, Montage);
		}
	}

	bool IsPlayingMontageByName(FName Name)
	{
		UAnimMontage* Montage = FindAnimMontage(Name);
		if (Montage)
		{
			return Montage_IsPlaying(Montage);
		}
		
		return false;
	}

protected:
	UFUNCTION()
	void MontageEnd(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void AnimNotify_MeeleAttack();

	UFUNCTION()
	void AnimNotify_Fire();
};
