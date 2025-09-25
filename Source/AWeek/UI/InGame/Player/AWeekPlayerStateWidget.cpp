// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekPlayerStateWidget.h"

UAWeekPlayerStateWidget::UAWeekPlayerStateWidget(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
}

void UAWeekPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*--------------INIT--------------*/
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthBar")));
	HungerBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HungerBar")));
}
