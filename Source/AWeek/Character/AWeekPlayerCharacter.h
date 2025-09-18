// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"

#include "AWeekCharacter.h"
#include "../Player/AWeekPlayerAnimInstance.h"

#include "../System/DamageInfo.h"
#include "../System/IDamageAble.h"

#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Kismet/KismetSystemLibrary.h"

#include "AWeekPlayerCharacter.generated.h"



UCLASS()
class AWEEK_API AAWeekPlayerCharacter : public AAWeekCharacter, public IDamageAble
{
	GENERATED_BODY()
public:
	AAWeekPlayerCharacter();

protected:
	/*--------------CAMERA--------------*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/*--------------ANIMINST--------------*/
	TObjectPtr<UAWeekPlayerAnimInstance> mAnimInst;

	/*--------------PAKOUR--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekPakourComponent> mPakour;

	/*--------------STAMINA--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekStaminaComponent> mStamina;
	
	/*--------------WEAPON--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAWeekWeaponComponent> mWeapon;

	/*--------------DAMAGE--------------*/
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UDamageSystemComponent> mDamageSystem;

	/*--------------PARTICLES--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> ParticleComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* FireEffect;

	/*--------------SOUNDS--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;

	/*--------------VARIABLES--------------*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSprint = false;

	UPROPERTY(EditAnywhere)
	float mWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float mFiringSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	float mSprintSpeed = 500.f;

	UPROPERTY(EditAnywhere)
	float mRunToStopTime = 0.75f;

	UPROPERTY(EditAnywhere)
	float mJumpVelocity = 550.f;

	UPROPERTY(EditAnywhere)
	float mSprintMinimumStamina = 25.f;

	UPROPERTY(VisibleAnywhere)
	float mSprintTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mSprintStaminaUsage = 25; // 달리기 초당 스태미나 소모량

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float mVaultStaminaUsage = 20;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Attack(const FInputActionValue& Value);
	void Fire();
	void EndFire();
	void SprintStart();
	void SprintCompleted();
	void ChangeWeapon();

public:
	virtual void VaultStart();
	virtual void VaultEnd();
	virtual void LedgeStart();
	virtual void LedgeEnd();
	virtual void ClimbStart();

	UFUNCTION()
	virtual void ClimbEnd();

	void AttackImpact();
	void FireBullet();

	UFUNCTION()
	void Die();

public:
	UFUNCTION(BlueprintCallable)
	void FootStepEffect(FName SocketName);
};
