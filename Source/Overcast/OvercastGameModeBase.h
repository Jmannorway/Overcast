// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OvercastGameModeBase.generated.h"

class UUserWidget;

/**
 * 
 */

UCLASS()
class OVERCAST_API AOvercastGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AOvercastGameModeBase();

	/*
		Both inheriting game modes need their own implementation of these functions.
		Hence, them being pure virtuals
	*/
	virtual void ReturnToMenu() PURE_VIRTUAL(AOvercastGameModeBase::ReturnToMenu, ;);
	virtual void QuitGame() PURE_VIRTUAL(AOvercastGameModeBase::QuitGame, ;);

	UFUNCTION(BlueprintCallable, Category = "File Handling")
		class UOvercastSaveGame* CreateNewGameSave() const;

	UFUNCTION(BlueprintCallable, Category = "File Handling")
		bool SaveExists() const;

	UFUNCTION(BlueprintCallable, Category = "User Interface")
		void SetWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "User Interface")
		void RemoveWidget();

private:

	UUserWidget* CurrentWidget;
};
