// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "Engine/Canvas.h"
#include "Player1.h"
#include "SpellSelector.h"

AGameHUD::AGameHUD()
{
	SpellTextures.SetNum(USpellSelector::GetSpellNumber());
	LastSpellIndex = 0;
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawTextureSimple(
		SpellHelpTexture,
		Canvas->SizeX - SpellHelpTexture->GetSurfaceWidth(),
		Canvas->SizeY - SpellHelpTexture->GetSurfaceHeight()
	);

	DrawTextureSimple(
		SpellTextures[LastSpellIndex],
		0.f,
		Canvas->SizeY - SpellHelpTexture->GetSurfaceHeight()
	);
}

void AGameHUD::SetHUDSpell(uint8 SpellIndex)
{
	LastSpellIndex = SpellIndex;
}
