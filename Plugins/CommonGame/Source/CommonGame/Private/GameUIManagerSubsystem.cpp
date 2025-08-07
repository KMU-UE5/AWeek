// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUIManagerSubsystem.h"

#include "GameUIPolicy.h"

void UGameUIManagerSubsystem::SwitchToPolicy(UGameUIPolicy* InPolicy)
{
	if (CurrentUIPolicy != InPolicy)
	{
		CurrentUIPolicy = InPolicy;
	}
}

void UGameUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!CurrentUIPolicy && !DefaultUIPolicyClass.IsNull())
	{
		TSubclassOf<UGameUIPolicy> PolicyClass = DefaultUIPolicyClass.LoadSynchronous();
		SwitchToPolicy(NewObject<UGameUIPolicy>(this, PolicyClass));
	}
}

void UGameUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	SwitchToPolicy(nullptr);
}

bool UGameUIManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// 혹시나 UI는 네트워크 서버에는 필요 없으므로 추가
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> ChildClasses;
		GetDerivedClasses(GetClass(), ChildClasses, false);

		return ChildClasses.Num() == 0;
	}

	return false;
}
