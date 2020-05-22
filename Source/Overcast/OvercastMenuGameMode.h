// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OvercastGameModeBase.h"
#include "OvercastMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OVERCAST_API AOvercastMenuGameMode : public AOvercastGameModeBase
{
	GENERATED_BODY()
	
public:

	AOvercastMenuGameMode();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	// Writes default data to a save and opens the first level
	UFUNCTION(BlueprintCallable, Category = "Overcast")
		void NewGame();

	// Moves to the last saved room
	UFUNCTION(BlueprintCallable, Category = "Overcast")
		bool ContinueGame();

	/*
		Implementations for PURE_VIRTUAL functions from OvercastGameModeBase.h
	*/

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		virtual void ReturnToMenu() override;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		virtual void QuitGame() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Overcast")
		TSubclassOf<class UUserWidget> StartingWidgetClass;
};
