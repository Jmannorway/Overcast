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
	static const uint8 mNumber = static_cast<uint8>(ESpellType::NUMBER);
	bool mbKeychain[mNumber];

public:
	// Overload increments the spell to the next unlocked spell
	void operator++();
	void operator++(int);

	// Converter functions, mostly for internal use
	ESpellType IndexToType(uint8 SpellIndex);
	uint8 TypeToIndex(ESpellType SpellType);

	// Getters for spellcasting, HUD updates, etc.
	UFUNCTION(BlueprintCallable) ESpellType GetSpellType() const;
	UFUNCTION(BlueprintCallable) uint8 GetSpellIndex() const;

	// Handy Blueprint SpellType enum setter and, spell index setter for mostly internal uses
	UFUNCTION(BlueprintCallable) void SetSpell(ESpellType SpellType);
	void SetSpell(uint8 SpellIndex);

	// Spell unlockers
	UFUNCTION(BlueprintCallable) void UnlockSpell(ESpellType SpellType);
	void UnlockSpell(uint8 SpellIndex);

	// Used to debug etc.
	UFUNCTION(BlueprintCallable) void UnlockAllSpells();

	// Default constructor locks every spell except for the first in the enum
	USpellSelector();
};