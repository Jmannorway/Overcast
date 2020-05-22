// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "OvercastSaveGame.h"
#include "OvercastGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/UserWidget.h"

AOvercastMenuGameMode::AOvercastMenuGameMode() { ; }

void AOvercastMenuGameMode::BeginPlay()
{
	// Add the selected widget
	if (StartingWidgetClass)
	{
		SetWidget(StartingWidgetClass);
	}

	// Set the input mode so that the player can interact with the menu
	if (auto PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->SetInputMode(FInputModeUIOnly());
		PlayerController->bShowMouseCursor = true;
	}

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay()"));
}

void AOvercastMenuGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveWidget();

	// Allow the player to interact with the game again
	if (auto PlayerController = UGameplayStatics::GetPlayerController(this, 0))
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("EndPlay()"));
}

void AOvercastMenuGameMode::NewGame()
{
	auto GameInstance = CastChecked<UOvercastGameInstance>(UGameplayStatics::GetGameInstance(this));

	// Write default data to a save slot (for specific values refer to OvercastSaveGame's constructor)
	UGameplayStatics::SaveGameToSlot(
		CreateNewGameSave(),
		GameInstance->GetSaveSlotString(),
		0);

	// Open first level
	UGameplayStatics::OpenLevel(this, GameInstance->GetFirstLevelName());
}

bool AOvercastMenuGameMode::ContinueGame()
{
	if (auto GameInstance = Cast<UOvercastGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (UGameplayStatics::DoesSaveGameExist(GameInstance->GetSaveSlotString(), 0))
		{
			// Load information from save and move to the next room
			auto OvercastSaveGame = CastChecked<UOvercastSaveGame>
				(UGameplayStatics::LoadGameFromSlot(GameInstance->GetSaveSlotString(), 0));

			UGameplayStatics::OpenLevel(this, OvercastSaveGame->SaveLevelName);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void AOvercastMenuGameMode::ReturnToMenu()
{
	SetWidget(StartingWidgetClass);
}

void AOvercastMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}
