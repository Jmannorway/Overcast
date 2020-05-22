// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OvercastGameInstance.generated.h"

class UOvercastSaveGame;

UCLASS()
class OVERCAST_API UOvercastGameInstance : public UGameInstance
{
	GENERATED_BODY()
		
public:

	UOvercastGameInstance();

	virtual void PostInitProperties() override;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FString GetSaveSlotString() const;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FName GetFirstLevelName() const;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FName GetMenuLevelName() const;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		TSubclassOf<UOvercastSaveGame> GetOvercastSaveGameClass();

private:
	UPROPERTY(EditAnywhere, Category = "Overcast")
		FString mSaveSlotString;

	UPROPERTY(EditAnywhere, Category = "Overcast")
		FName mFirstLevelName;

	UPROPERTY(EditAnywhere, Category = "Overcast")
		FName mMenuLevelName;

	UPROPERTY(EditAnywhere, Category = "Overcast")
		TSubclassOf<UOvercastSaveGame> mOvercastSaveGameClass;
};