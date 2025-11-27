// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekGameOverPanel.h"
#include "../../System/Score/ScoreSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"



void UAWeekGameOverPanel::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (SubmitScoreButton)
    {
        SubmitScoreButton->OnClicked.AddDynamic(this, &UAWeekGameOverPanel::HandleSubmitScoreClicked);
    }
    if (PlayerNameTextBox)
    {
        PlayerNameTextBox->OnTextChanged.AddDynamic(this, &UAWeekGameOverPanel::HandleTextChanged);
    }
}

void UAWeekGameOverPanel::InitializeGameOverPanel()
{
    UScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UScoreSubsystem>();
    if (!ScoreSubsystem) return;

    CurrentScore = ScoreSubsystem->CurrentEntry.Score;
    CurrentKillCount = ScoreSubsystem->CurrentEntry.ZombieKillCount;
    CurrentDay = ScoreSubsystem->CurrentEntry.Day;

    if (ScoreText)
    {
        ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), CurrentScore)));
    }
    if (KillCountText)
    {
        KillCountText->SetText(FText::FromString(FString::Printf(TEXT("Zombie Kills: %d"), CurrentKillCount)));
    }
    if (DayText)
    {
        DayText->SetText(FText::FromString(FString::Printf(TEXT("Day: %d"), CurrentDay)));
    }

    if (PlayerNameTextBox)
    {
        PlayerNameTextBox->SetText(FText::FromString(TEXT("")));
    }

    if (SubmitScoreButton)
    {
        SubmitScoreButton->SetIsEnabled(true);
    }
}

void UAWeekGameOverPanel::HandleSubmitScoreClicked()
{
    if (!PlayerNameTextBox) return;

    FString PlayerName = PlayerNameTextBox->GetText().ToString();
    if (PlayerName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Player name is empty!"));
        return;
    }

    UScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UScoreSubsystem>();
    if (ScoreSubsystem)
    {
        ScoreSubsystem->SaveRankEntry(PlayerName);
        UE_LOG(LogTemp, Log, TEXT("Saved Score: %d, KillCount: %d, Day: %d, Player: %s"),
            CurrentScore, CurrentKillCount, CurrentDay, *PlayerName);
    }

    if (SubmitScoreButton)
    {
        SubmitScoreButton->SetIsEnabled(false);
    }
    DeactivateWidget();
    RemoveFromParent();
}

void UAWeekGameOverPanel::HandleTextChanged(const FText& Text)
{
    FString NewText = Text.ToString();

    if (NewText.Len() > 12)
    {
        NewText = NewText.Left(12);
        PlayerNameTextBox->SetText(FText::FromString(NewText));
    }
}