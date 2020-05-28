// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastMainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Player1.h"
#include "GameFramework/PlayerController.h"
#include "SpellSelector.h"
#include "Checkpoint.h"
#include "OvercastSaveGame.h"
#include "OvercastGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SpellScroll.h"

// This might be insanely unsafe, idk??? Undefined behavior? Hopefully not lol
AOvercastMainGameMode::AOvercastMainGameMode()
{ 
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Auto respawn variable defaults
	bAutoRespawn = true;
	AutoRespawnDelay = 2.f;
	AutoRespawnTimer = 0.f;
}

void AOvercastMainGameMode::BeginPlay()
{
	if (!ReadGame())
	{
		OvercastSaveGame = CreateNewGameSave();
		bSaveGameIsValid = true;
		UE_LOG(LogTemp, Warning, TEXT("ReadGame failed in BeginPlay created a new save game"));

		if (!WriteGame())
			UE_LOG(LogTemp, Warning, TEXT("WriteGame succeeded"))
		else
			UE_LOG(LogTemp, Warning, TEXT("WriteGame failed in BeginPlay trying to save the new game save"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReadGame succeeded!"))
	}

	if (bAutoLoadGame)
	{
		if (!LoadGame())
			UE_LOG(LogTemp, Warning, TEXT("LoadGame failed in BeginPlay"))
		else
			UE_LOG(LogTemp, Warning, TEXT("LoadGame succeeded!"))
	}
}

void AOvercastMainGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (!WriteGame())
		UE_LOG(LogTemp, Warning, TEXT("WriteGame failed in EndPlay"))
	else
		UE_LOG(LogTemp, Warning, TEXT("WriteGame succeeded!"))
}

void AOvercastMainGameMode::Tick(float DeltaTime)
{
	// Automatically respawn if player if dead or doesn't exist
	if (bAutoRespawn)
	{
		auto Player = Cast<APlayer1>(UGameplayStatics::GetPlayerPawn(this, 0));

		if (!Player || Player->GetPlayerMovementState() == EPlayerMovementState::Dead)
		{
			AutoRespawnTimer += DeltaTime;

			UE_LOG(LogTemp, Warning, TEXT("Auto Respawn Timer: %f"), AutoRespawnTimer);

			if (AutoRespawnTimer >= AutoRespawnDelay)
				Respawn(bDestroyWhenRespawning);
		}
		else
		{
			AutoRespawnTimer = 0.f;
		}
	}
}

void AOvercastMainGameMode::ReturnToMenu()
{
	if (!WriteGame())
		UE_LOG(LogTemp, Warning, TEXT("WriteGame failed in ReturnToMenu"))
	else
		UE_LOG(LogTemp, Warning, TEXT("WriteGame succeeded!"))

	UGameplayStatics::OpenLevel(this, CastChecked<UOvercastGameInstance>(UGameplayStatics::GetGameInstance(this))->GetMenuLevelName());
}

void AOvercastMainGameMode::QuitGame()
{
	WriteGame();
	UKismetSystemLibrary::QuitGame(this, UGameplayStatics::GetPlayerController(this, 0), EQuitPreference::Quit, false);
}

bool AOvercastMainGameMode::TogglePause()
{
	if (UGameplayStatics::IsGamePaused(this))
	{
		RemoveWidget();
		UGameplayStatics::SetGamePaused(this, false);
		return false;
	}
	else
	{
		SetWidget(PauseWidgetClass);
		UGameplayStatics::SetGamePaused(this, true);
		return true;
	}
}

const UOvercastSaveGame* AOvercastMainGameMode::GetCurrentSaveGame() const
{
	return OvercastSaveGame;
}

bool AOvercastMainGameMode::SaveGame()
{
	if (!bSaveGameIsValid)
		OvercastSaveGame = CreateNewGameSave();

	// Save level
	OvercastSaveGame->SaveLevelName = *UGameplayStatics::GetCurrentLevelName(this);

	// Save values if the player exists
	if (auto Player = Cast<APlayer1>(UGameplayStatics::GetPlayerCharacter(this, 0)))
		OvercastSaveGame->SaveUnlockedSpells = Player->SpellSelector->GetUnlockedSpells();
	else
		return false;

	return true;
}

void AOvercastMainGameMode::SaveGameLevelCheckpoint(FName LevelName, int32 CheckpointIndex)
{
	if (!bSaveGameIsValid)
		OvercastSaveGame = CreateNewGameSave();

	OvercastSaveGame->SaveLevelName = LevelName;
	OvercastSaveGame->SaveCheckpointIndex = CheckpointIndex;

	// Save values if the player exists
	if (auto Player = Cast<APlayer1>(UGameplayStatics::GetPlayerCharacter(this, 0)))
		OvercastSaveGame->SaveUnlockedSpells = Player->SpellSelector->GetUnlockedSpells();
}

bool AOvercastMainGameMode::LoadGame()
{
	if (bSaveGameIsValid)
	{
		if (*UGameplayStatics::GetCurrentLevelName(this) == OvercastSaveGame->SaveLevelName)
		{
			// Spawn the player
			Respawn(bDestroyWhenRespawning);
		}
		else
		{
			// Open saved level (ps: Dereference operator on FString converts it to FName)
			UGameplayStatics::OpenLevel(this, OvercastSaveGame->SaveLevelName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Load failed: Save Game boolean was false"));
		return false;
	}

	return true;
}

bool AOvercastMainGameMode::WriteGame()
{
	return UGameplayStatics::SaveGameToSlot(
		OvercastSaveGame,
		CastChecked<UOvercastGameInstance>(UGameplayStatics::GetGameInstance(this))->GetSaveSlotString(),
		0);
}

bool AOvercastMainGameMode::ReadGame()
{
	if (SaveExists())
	{
		OvercastSaveGame = CastChecked<UOvercastSaveGame>(UGameplayStatics::LoadGameFromSlot(CastChecked<UOvercastGameInstance>(UGameplayStatics::GetGameInstance(this))->GetSaveSlotString(), 0));
		bSaveGameIsValid = true;
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read game"));
		return false;
	}
}

void AOvercastMainGameMode::Respawn(bool bDestroyPlayer)
{
	// Find the best location to spawn at
	FVector SpawnLocation = FindCheckpointSpawnLocation(OvercastSaveGame->SaveCheckpointIndex);

	if (SpawnLocation == FVector::ZeroVector)
	{
		SpawnLocation = FindPlayerStartSpawnLocation();
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	// Destroy the player if they exists, set to nullptr
	if (bDestroyPlayer && PlayerPawn)
	{
		PlayerPawn->Destroy();
		PlayerPawn = nullptr;
	}

	// Spawn player by either moving an existing player instance or by spawning a new one
	APlayer1* Player;

	if (PlayerPawn)
	{
		Player = Cast<APlayer1>(PlayerPawn);
		Player->SetActorLocation(SpawnLocation);
		Player->SpellSelector->SetUnlockedSpells(OvercastSaveGame->SaveUnlockedSpells);
	}
	else
	{
		Player = GetWorld()->SpawnActor<APlayer1>(DefaultPawnClass, SpawnLocation, FRotator::ZeroRotator);
		Player->SpellSelector->SetUnlockedSpells(OvercastSaveGame->SaveUnlockedSpells);
	}

	if (Player)
	{
		UGameplayStatics::GetPlayerController(this, 0)->Possess(Player);
		Player->SetPlayerMovementState(EPlayerMovementState::Normal);
	}
}

FVector AOvercastMainGameMode::FindCheckpointSpawnLocation(int32 CheckpointIndex)
{
	// Get the location of a checkpoint
	TArray<AActor*> Checkpoint;
	UGameplayStatics::GetAllActorsOfClass(this, CheckpointClass, Checkpoint);

	bool bCorrectCheckpoint = false;
	ACheckpoint* CastCheckpoint;

	for(auto i : Checkpoint)
	{
		CastCheckpoint = CastChecked<ACheckpoint>(i);

		if (CastCheckpoint->GetCheckpointIndex() == CheckpointIndex)
		{
			return CastCheckpoint->GetSpawnLocation();
			UE_LOG(LogTemp, Warning, TEXT("Got spawn location"));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Didn't get spawn location"));
	return FVector::ZeroVector;
}

FVector AOvercastMainGameMode::FindPlayerStartSpawnLocation()
{
	// Get the location of a player start
	if (auto PlayerStart = FindPlayerStart(UGameplayStatics::GetPlayerController(this, 0)))
	{
		return PlayerStart->GetActorLocation();
	}
	else
	{
		return FVector::ZeroVector;
	}
}
