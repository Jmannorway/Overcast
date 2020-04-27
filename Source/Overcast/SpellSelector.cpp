// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellSelector.h"

void USpellSelector::UnlockSpell(uint8 SpellIndex)
{
	if (SpellIndex < mNumber)
		mbKeychain[SpellIndex] = true;
}

void USpellSelector::UnlockSpell(ESpellType SpellType)
{
	if (uint8 SpellIndex = TypeToIndex(SpellType) < mNumber)
		mbKeychain[SpellIndex] = true;
}

USpellSelector::USpellSelector()
{
	mSpell.i = 0;

	for (bool& i : mbKeychain)
		i = false;

	mbKeychain[0] = true;

	UE_LOG(LogTemp, Warning, TEXT("%i"), mNumber);
}

ESpellType USpellSelector::IndexToType(uint8 SpellIndex)
{
	return (SpellIndex < mNumber) ? static_cast<ESpellType>(SpellIndex) : ESpellType::INVALID;
}

uint8 USpellSelector::TypeToIndex(ESpellType SpellType)
{
	if (SpellType != ESpellType::NUMBER && SpellType != ESpellType::INVALID)
		return static_cast<uint8>(SpellType);
	else
		return static_cast<uint8>(ESpellType::INVALID);
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
	if (SpellType != ESpellType::NUMBER && SpellType != ESpellType::INVALID && mbKeychain[TypeToIndex(SpellType)])
		mSpell.t = SpellType;
}

void USpellSelector::SetSpell(uint8 SpellIndex)
{
	if (SpellIndex < mNumber && mbKeychain[SpellIndex])
		mSpell.i = SpellIndex;
}

void USpellSelector::UnlockAllSpells()
{
	for (bool& i : mbKeychain)
		i = true;
}

void USpellSelector::operator++()
{
	do {
		mSpell.i = (mSpell.i + 1) % mNumber;
	} while (!mbKeychain[mSpell.i]);
}

void USpellSelector::operator++(int)
{
	++(*this);
}