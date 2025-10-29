// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API ABaseEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Initialize();
	UFUNCTION(BlueprintCallable)
	void SetBlackboardValues();

public:
	ABaseEnemyAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttackTargetKeyName = "AttackTarget";
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBehaviorTree* BTAsset;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBlackboardData* BBAsset;
	UPROPERTY(BlueprintReadOnly)
	UBlackboardComponent* BBComponent;
};

