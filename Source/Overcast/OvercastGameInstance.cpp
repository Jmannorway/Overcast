// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastGameInstance.h"

UOvercastGameInstance::UOvercastGameInstance() { ; }

FString UOvercastGameInstance::GetSaveSlotString() const
{
	return mSaveSlotString;
}

FName UOvercastGameInstance::GetFirstLevelName() const
{
	return mFirstLevelName;
}

FName UOvercastGameInstance::GetMenuLevelName() const
{
	return mMenuLevelName;
}

TSubclassOf<UOvercastSaveGame> UOvercastGameInstance::GetOvercastSaveGameClass()
{
	return mOvercastSaveGameClass;
}
