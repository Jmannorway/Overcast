// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OvercastGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCAST_API AOvercastGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

    AOvercastGameMode();

    virtual void Tick(float DeltaTime) override;

    // Kills the player if still alive and respawn them
    UFUNCTION(BlueprintCallable, Category = "Overcast")
        void Respawn();

    // Save current values and return to menu
    UFUNCTION(BlueprintCallable, Category = "Overcast")
        void ReturnToMenu();

    UFUNCTION(BlueprintCallable, Category = "Overcast")
        void SetCheckpointIndex(int32 NewCheckpointIndex);

protected:

    // The current level being played in the level sequence
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overcast")
        int32 CurrentLevelIndex;

    // The current checkpoint in the level being used
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overcast")
        int32 CurrentCheckpointIndex;

    // The sequence of levels as they appear in the game
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overcast")
        TArray<FName> LevelSequence;

    // The name of the menu level
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overcast")
        FName MenuLevelName;

    // A class reference to the checkpoint object
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overcast")
        TSubclassOf<class ACheckpoint> CheckpointClass;

    // Subroutines of respawn
    APawn* SpawnAtCheckpoint();
    APawn* SpawnAtPlayerStart();
    APawn* SpawnAtZero();
};
