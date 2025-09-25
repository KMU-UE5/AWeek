// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "../../System/GameEventMessageSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "AWeekDayWidget.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekDayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAWeekDayWidget(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UTextBlock> Label;
	FGameEventMessageListenerHandle DayChangeHandle;

protected:
	virtual void NativeConstruct() override;
	
};
