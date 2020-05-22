// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "OvercastSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class OVERCAST_API UOvercastSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UOvercastSaveGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
		FName SaveLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
		int32 SaveCheckpointIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
		TArray<bool> SaveUnlockedSpells;
};
