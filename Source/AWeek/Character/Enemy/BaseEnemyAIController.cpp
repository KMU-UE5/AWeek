// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAIController.h"
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include"Navigation/CrowdFollowingComponent.h"

void ABaseEnemyAIController::Initialize()
{
	RunBehaviorTree(BTAsset);
	UseBlackboard(BBAsset, BBComponent);
}

void ABaseEnemyAIController::SetBlackboardValues()
{
	if (BBComponent == nullptr)
		return;
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	BBComponent->SetValueAsObject(AttackTargetKeyName, PlayerCharacter);
}

ABaseEnemyAIController::ABaseEnemyAIController(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
}

void ABaseEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	UCrowdFollowingComponent* CrowdFollwingComponent = FindComponentByClass<UCrowdFollowingComponent>();
	if (CrowdFollwingComponent)
	{
		CrowdFollwingComponent->SetCrowdSeparation(true);
		CrowdFollwingComponent->SetCrowdSeparationWeight(50.0f);
		CrowdFollwingComponent->SetCrowdAvoidanceRangeMultiplier(1.1f);
		CrowdFollwingComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);
	}
}

 