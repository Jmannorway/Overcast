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

protected:
	
	UPROPERTY(EditAnywhere, Category = "Overcast")
		TArray<FName> LevelSequence;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		void Save(int32 LevelIndex, int32 CheckpointIndex);

	class UOvercastSaveHandler* SaveHandler;
};
