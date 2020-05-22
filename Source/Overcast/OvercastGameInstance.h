// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OvercastGameInstance.generated.h"

class UOvercastSaveGame;

/*
	A macro that defines a protected member, a public constant reference to the protected member, and a getter
	Sadly this didn't work since the c++ preprocessor conflicts with the UHT (Unreal Header Tool).
	It only remains here as a reminder.
*/
#define DEFINE_GAMEINSTANCE_SET(TYPE, NAME) \
	private:\
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overcast)\
		TYPE m ## NAME;\
	public:\
	const TYPE& NAME = m ## NAME;\
	UFUNCTION(BlueprintCallable, Category = Overcast)\
		TYPE Get ## NAME() const {return m ## NAME;}

UCLASS()
class OVERCAST_API UOvercastGameInstance : public UGameInstance
{
	GENERATED_BODY()
		
public:

	UOvercastGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FString GetSaveSlotString() const;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FName GetFirstLevelName() const;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		FName GetMenuLevelName() const;

	UFUNCTION(BlueprintCallable, Category = "Overcast")
		TSubclassOf<UOvercastSaveGame> GetOvercastSaveGameClass();

	const FString& SaveSlotString = mSaveSlotString;
	const FName& FirstLevelName = mFirstLevelName;
	const FName& MenuLevelName = mMenuLevelName;
	const TSubclassOf<UOvercastSaveGame>& OvercastSaveGameClass = mOvercastSaveGameClass;

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

#undef DEFINE_GAMEINSTANCE_SET