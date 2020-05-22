// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OvercastGameModeBase.h"
#include "OvercastMainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCAST_API AOvercastMainGameMode : public AOvercastGameModeBase
{
	GENERATED_BODY()
	
public:

	AOvercastMainGameMode();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	/*
		File handling functions for overcast
		SaveGame and LoadGame should be used in blueprint by various game
		assets to do saving and loading

		ReadGame & WriteGame are only public in the testing phases and
		shouldn't be used by any other assets of the game as the game mode
		should take care of this process on its own
	*/

	UFUNCTION(BlueprintCallable, Category = "File Handling")
		bool SaveGame();

	UFUNCTION(BlueprintCallable, Category = "File Handling")
		bool LoadGame();

	UFUNCTION(BlueprintCallable, Category = "File Handling")
		bool WriteGame();

	UFUNCTION(BlueprintCallable, Category = "File Handling")
		bool ReadGame();

	/*
		Implementations for PURE_VIRTUAL functions from OvercastGameModeBase.h
	*/
	UFUNCTION(BlueprintCallable, Category = "Overcast")
		virtual void ReturnToMenu() override;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		virtual void QuitGame() override;

	// A self explanatory function
	UFUNCTION(BlueprintCallable, Category = "Overcast")
		bool TogglePause();

protected:

	// A SaveGame instance to store values in when saving and read values from when loading
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "File Handling")
		class UOvercastSaveGame* OvercastSaveGame;

	// A variable that keeps track of the validity of the save game instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "File Handling")
		bool bSaveGameIsValid;

	// Whether or not the game should automatically be loaded
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "File Handling")
		bool bAutoLoadGame;

	/*
		Variables for pausing
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overcast")
		TSubclassOf<class UUserWidget> PauseWidgetClass;

	/*
		Variables and functions for spawning/respawning
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Overcast")
		TSubclassOf<class ACheckpoint> CheckpointClass;

	// Functions used to find locations to respawn in
	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FVector FindCheckpointSpawnLocation(int32 CheckpointIndex);

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FVector FindPlayerStartSpawnLocation();

	// Spawns player pawn at given position
	void Respawn(bool bDestroyPlayer);
};
