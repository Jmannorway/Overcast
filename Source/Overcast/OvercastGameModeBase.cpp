
// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "OvercastGameInstance.h"
#include "OvercastSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SpellSelector.h"

AOvercastGameModeBase::AOvercastGameModeBase() { ; }

UOvercastSaveGame* AOvercastGameModeBase::CreateNewGameSave() const
{
	// Create a new save game if none was made before
	auto GameInstance = CastChecked<UOvercastGameInstance>(UGameplayStatics::GetGameInstance(this));
	auto SaveGame = CastChecked<UOvercastSaveGame>(
		UGameplayStatics::CreateSaveGameObject(GameInstance->GetOvercastSaveGameClass()));

	SaveGame->SaveCheckpointIndex = -1;
	SaveGame->SaveLevelName = GameInstance->GetFirstLevelName();
	SaveGame->SaveUnlockedSpells = USpellSelector::GetDefaultUnlockedSpells();
	SaveGame->SaveUnlockedSpells[1] = true;

	return SaveGame;
}

bool AOvercastGameModeBase::SaveExists() const
{
	return UGameplayStatics::DoesSaveGameExist(
		CastChecked<UOvercastGameInstance>(UGameplayStatics::GetGameInstance(this))->GetSaveSlotString(),
		0);
}

void AOvercastGameModeBase::SetWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	RemoveWidget();

	if (NewWidgetClass)
	{
		auto NewWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);

		if (NewWidget)
		{
			CurrentWidget = NewWidget;
			CurrentWidget->AddToViewport();
		}
	}
}

void AOvercastGameModeBase::RemoveWidget()
{
	if (CurrentWidget)
	{
		if (CurrentWidget->IsInViewport())
		{
			CurrentWidget->RemoveFromParent();
		}

		CurrentWidget = nullptr;
	}
}
