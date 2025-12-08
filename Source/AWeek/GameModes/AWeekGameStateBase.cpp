// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameStateBase.h"

#include "AWeekExperienceManagerComponent.h"

AAWeekGameStateBase::AAWeekGameStateBase(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UAWeekExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));
}
