// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellSelector.h"

void USpellSelector::UnlockSpell(uint8 SpellIndex)
{
	if (SpellIndex < mbUnlockedSpells.Num())
		mbUnlockedSpells[SpellIndex] = true;
}

void USpellSelector::UnlockSpell(ESpellType SpellType)
{
	if (uint8 SpellIndex = TypeToIndex(SpellType) < mbUnlockedSpells.Num())
		mbUnlockedSpells[SpellIndex] = true;
}

USpellSelector::USpellSelector()
{
	Initialize();
	mSpell.i = 0;
	mbUnlockedSpells[0] = true;
	UE_LOG(LogTemp, Warning, TEXT("%i"), mbUnlockedSpells.Num());
}

ESpellType USpellSelector::IndexToType(uint8 SpellIndex)
{
	return (SpellIndex < mbUnlockedSpells.Num()) ? static_cast<ESpellType>(SpellIndex) : ESpellType::INVALID;
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
	if (SpellType != ESpellType::NUMBER && SpellType != ESpellType::INVALID && mbUnlockedSpells[TypeToIndex(SpellType)])
		mSpell.t = SpellType;
}

void USpellSelector::SetSpell(uint8 SpellIndex)
{
	if (SpellIndex < mbUnlockedSpells.Num() && mbUnlockedSpells[SpellIndex])
		mSpell.i = SpellIndex;
}

void USpellSelector::UnlockAllSpells()
{
	for (bool& i : mbUnlockedSpells)
		i = true;
}

void USpellSelector::operator++()
{
	NextSpell();
}

void USpellSelector::operator++(int)
{
	NextSpell();
}

void USpellSelector::operator--()
{
	PreviousSpell();
}

void USpellSelector::operator--(int)
{
	PreviousSpell();
}

void USpellSelector::NextSpell()
{
	do
		mSpell.i = (mSpell.i + 1) % mbUnlockedSpells.Num();
	while (!mbUnlockedSpells[mSpell.i]);
}

void USpellSelector::PreviousSpell()
{
	do
		if (mSpell.i > 0)
			mSpell.i--;
		else
			mSpell.i = mbUnlockedSpells.Num() - 1;
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

void USpellSelector::Initialize()
{
	mbUnlockedSpells.SetNum(USpellSelector::GetSpellNumber());

	for (bool& i : mbUnlockedSpells)
		i = false;
}