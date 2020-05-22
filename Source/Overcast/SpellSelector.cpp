// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellSelector.h"

/*
	Constructors
*/

USpellSelector::USpellSelector()
{
	Initialize();
	mSpell.i = 0;
	mbUnlockedSpells[0] = true;
}

USpellSelector::USpellSelector(uint8 UnlockedSpellIndex)
{
	Initialize();
	mSpell.i = UnlockedSpellIndex;
	mbUnlockedSpells[UnlockedSpellIndex] = true;
}

USpellSelector::USpellSelector(TArray<bool> UnlockedSpells)
{
	Initialize();
	mbUnlockedSpells = UnlockedSpells;

	for (uint8 i = 0; i < USpellSelector::GetSpellNumber(); i++)
		if (mbUnlockedSpells[i])
			mSpell.i = 0;
}

void USpellSelector::Initialize()
{
	mbUnlockedSpells.SetNum(USpellSelector::GetSpellNumber());

	for (bool& i : mbUnlockedSpells)
		i = false;
}

/*
	Public functions
*/

void USpellSelector::UnlockSpell(uint8 SpellIndex)
{
	if (SpellIndex < USpellSelector::GetSpellNumber())
		mbUnlockedSpells[SpellIndex] = true;
}

void USpellSelector::UnlockSpell(ESpellType SpellType)
{
	uint8 SpellIndex = TypeToIndex(SpellType);
	mbUnlockedSpells[SpellIndex] = true;
}

ESpellType USpellSelector::GetSpellType() const
{
	return mSpell.t;
}

uint8 USpellSelector::GetSpellIndex() const
{
	return mSpell.i;
}

void USpellSelector::SetSpell(ESpellType SpellType)
{
	SetSpell(TypeToIndex(SpellType));
}

void USpellSelector::SetSpell(uint8 SpellIndex)
{
	if (SpellIndex < USpellSelector::GetSpellNumber() && mbUnlockedSpells[SpellIndex])
		mSpell.i = SpellIndex;
}

void USpellSelector::UnlockAllSpells()
{
	for (bool& i : mbUnlockedSpells)
		i = true;
}

void USpellSelector::NextSpell()
{
	do
		mSpell.i = (mSpell.i + 1) % USpellSelector::GetSpellNumber();
	while (!mbUnlockedSpells[mSpell.i]);
}

void USpellSelector::PreviousSpell()
{
	do
		if (mSpell.i > 0)
			mSpell.i--;
		else
			mSpell.i = USpellSelector::GetSpellNumber() - 1;
	while (!mbUnlockedSpells[mSpell.i]);
}

const TArray<bool> USpellSelector::GetUnlockedSpells() const
{
	return mbUnlockedSpells;
}

void USpellSelector::SetUnlockedSpells(const TArray<bool> UnlockedSpells)
{
	if (UnlockedSpells.Num() <= mbUnlockedSpells.Num())
		for (int32 i = UnlockedSpells.Num() - 1; i >= 0; i--)
			mbUnlockedSpells[i] = UnlockedSpells[i];

	if (!mbUnlockedSpells[mSpell.i])
		NextSpell();
}

/*
	Static functions start here
*/

uint8 USpellSelector::GetSpellNumber()
{
	return static_cast<uint8>(ESpellType::NUMBER);
}

TArray<bool> USpellSelector::GetDefaultUnlockedSpells()
{
	TArray<bool> DefaultUnlockedSpells;
	DefaultUnlockedSpells.SetNum(USpellSelector::GetSpellNumber());
	DefaultUnlockedSpells[0] = true;

	return DefaultUnlockedSpells;
}

ESpellType USpellSelector::IndexToType(uint8 SpellIndex)
{
	return SpellIndex < USpellSelector::GetSpellNumber() ? static_cast<ESpellType>(SpellIndex) : ESpellType::INVALID;
}

uint8 USpellSelector::TypeToIndex(ESpellType SpellType)
{
	if (SpellType != ESpellType::NUMBER && SpellType != ESpellType::INVALID)
		return static_cast<uint8>(SpellType);
	else
		return static_cast<uint8>(ESpellType::INVALID);
}