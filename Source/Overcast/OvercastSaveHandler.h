// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class USpellSelector;

/**
 * 
 */
class OVERCAST_API OvercastSaveHandler
{
public:

	TArray<bool> bUnlockedSpells;
	int32 CheckpointIndex;
	int32 LevelIndex;

	bool WriteSave(const char* file) const;
	bool ReadSave(const char* file);

	OvercastSaveHandler();
	~OvercastSaveHandler();
};
