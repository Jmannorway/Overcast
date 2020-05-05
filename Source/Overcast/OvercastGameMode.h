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

    UPROPERTY(EditAnywhere, Category = "Overcast")
        int32 CurrentLevelIndex;

    UPROPERTY(EditAnywhere, Category = "Overcast")
        int32 CurrentCheckpointIndex;

    UPROPERTY(EditAnywhere, Category = "Overcast")
        TArray<FName> LevelSequence;

    UPROPERTY(EditAnywhere, Category = "Overcast")
        TSubclassOf<class ACheckpoint> CheckpointClass;

    UFUNCTION(BlueprintCallable, Category = "Overcast")
        void Respawn();

    // Subroutines of respawn
    APawn* SpawnAtCheckpoint();
    APawn* SpawnAtPlayerStart();
    APawn* SpawnAtZero();
};
