// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpellSelector.generated.h"

/**
 * An invisible inventory system for spells used in game
 */

UENUM(BlueprintType)
enum class ESpellType : uint8
{
	Rain	UMETA(DisplayName = "Rain"),
	Wind	UMETA(DisplayName = "Wind"),
	Shade	UMETA(DisplayName = "Shade"),
	NUMBER,
	INVALID
};

UCLASS(BlueprintType)
class OVERCAST_API USpellSelector : public UObject
{
	GENERATED_BODY()

private:
	union { ESpellType t; uint8 i; } mSpell;
	TArray<bool> mbUnlockedSpells;

	// Initialization function for the constructor
	void Initialize();

public:

	// Default constructor locks every spell except for the first in the enum
	USpellSelector();
	USpellSelector(uint8 UnlockedSpellIndex);
	USpellSelector(TArray<bool> UnlockedSpells);

	// More user friendly incremental functions
	UFUNCTION(BlueprintCallable) void NextSpell();
	UFUNCTION(BlueprintCallable) void PreviousSpell();

	// Getters for spellcasting, HUD updates, etc.
	UFUNCTION(BlueprintCallable) ESpellType GetSpellType() const;
	UFUNCTION(BlueprintCallable) uint8 GetSpellIndex() const;

	// Handy Blueprint SpellType enum setter and, spell index setter for mostly internal uses
	UFUNCTION(BlueprintCallable) void SetSpell(ESpellType SpellType);
	void SetSpell(uint8 SpellIndex);

	// Spell unlockers
	UFUNCTION(BlueprintCallable) void UnlockSpell(ESpellType SpellType);
	void UnlockSpell(uint8 SpellIndex);
	UFUNCTION(BlueprintCallable) void UnlockAllSpells();

	// Unlocked spells getter & setter
	const TArray<bool> GetUnlockedSpells() const;
	void SetUnlockedSpells(const TArray<bool> UnlockedSpells);

	// Handy static functions for saving purposes
	UFUNCTION(BlueprintCallable)
		static uint8 GetSpellNumber();

	UFUNCTION(BlueprintCallable)
		static TArray<bool> GetDefaultUnlockedSpells();

	// Conversion functions
	UFUNCTION(BlueprintCallable)
		static ESpellType IndexToType(uint8 SpellIndex);

	UFUNCTION(BlueprintCallable)
		static uint8 TypeToIndex(ESpellType SpellType);
};