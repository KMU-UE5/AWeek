// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/UI/Interaction/AWeekInteractionWidget.h"

#include "AWeek/Character/AWeekPlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AWeek/Interfaces/AWeekInteractionInterface.h"
#include "AWeek/UI/AWeekGameUIManager.h"

UAWeekInteractionWidget::UAWeekInteractionWidget(): CurrentInteractionDuration(0.0f)
{
}

void UAWeekInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
	if (AAWeekPlayerCharacter* PlayerCharacter = Cast<AAWeekPlayerCharacter>(GetOwningPlayerPawn()))
	{
		PlayerCharacter->OnInteractionTargetChanged.AddUObject(this, &UAWeekInteractionWidget::OnInteractionTargetChanged);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s: PlayerCharacter is null!"), *FString(__FUNCTION__));
	}

	SetVisibility(ESlateVisibility::Collapsed);
}

void UAWeekInteractionWidget::OnInteractionTargetChanged(const FAWeekInteractableData* InteractableData)
{
	if (InteractableData)
	{
		SetVisibility(ESlateVisibility::Visible);
		UpdateWidget(InteractableData);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAWeekInteractionWidget::UpdateWidget(const FAWeekInteractableData* FInteractableData) const
{
	if (!FInteractableData)
	{
		return;
	}
	switch (FInteractableData->InteractableType)
	{
	case EAWeekInteractableType::Pickup:
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
		KeyPressText->SetText(FText::FromString("Press"));

		if (FInteractableData->Quantity < 2)
		{
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"),
				FInteractableData->Quantity));
			QuantityText->SetVisibility(ESlateVisibility::Visible);
		}
		break;

	case EAWeekInteractableType::NonplayerCharacter:
		break;

	case EAWeekInteractableType::Device:
		break;

	case EAWeekInteractableType::Toggle:
		break;

	case EAWeekInteractableType::Chest:
		KeyPressText->SetText(FText::FromString("Press"));
		InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);

		break;

	default:
		break;
	}

	ActionText->SetText(FInteractableData->Action);
	NameText->SetText(FInteractableData->Name);
}

float UAWeekInteractionWidget::UpdateInteractionProgress()
{
	return 0.0f;
}

