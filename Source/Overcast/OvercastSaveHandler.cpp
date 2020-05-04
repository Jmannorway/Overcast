// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastSaveHandler.h"
#include "SpellSelector.h"
#include <fstream>

bool OvercastSaveHandler::WriteSave(const char* file) const
{
	std::ofstream save;
	save.open(file);

	if (save.is_open())
	{
		for (bool i : bUnlockedSpells) save << i << " ";
		save << CheckpointIndex << " " << LevelIndex;

		save.close();
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't write to save"))
		return false;
	}
}

bool OvercastSaveHandler::ReadSave(const char* file)
{
	std::ifstream save;
	save.open(file);

	if (save.is_open())
	{
		for (bool i : bUnlockedSpells) save >> i;
		save >> CheckpointIndex >> LevelIndex;

		save.close();
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't read from save"))
		return false;
	}
}

OvercastSaveHandler::OvercastSaveHandler()
{
	CheckpointIndex = 0;
	LevelIndex = 0;
}

OvercastSaveHandler::~OvercastSaveHandler()
{
}
