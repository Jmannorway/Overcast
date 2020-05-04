// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastGameMode.h"
#include "OvercastSaveHandler.h"
#include "Player1.h"
#include "Kismet/GameplayStatics.h"
#include "SpellSelector.h"

void AOvercastGameMode::Save(int32 LevelIndex, int32 CheckpointIndex)
{
	if (auto Player = Cast<APlayer1>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		// ERROR WHY!?
		//SaveHandler.UnlockedSpells = Player->GetSpellSelector()->GetUnlockedSpells();
		
	}
}
