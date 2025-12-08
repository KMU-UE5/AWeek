// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AWeekActivatableWidget.h"
#include "AWeekHUDLayout.generated.h"

/**
 * 
 */
UCLASS()
class AWEEK_API UAWeekHUDLayout : public UAWeekActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
protected:
	void HandleEscapeAction();
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCommonActivatableWidget> EscapeMenuClass;
};