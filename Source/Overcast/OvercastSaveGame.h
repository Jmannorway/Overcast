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

	uint32 CheckpointIndex;

	FString LevelName;

};
