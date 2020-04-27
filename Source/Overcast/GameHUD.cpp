// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "Engine/Canvas.h"

AGameHUD::AGameHUD()
{
	Spell.SetNum(3);
}

void AGameHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawTextureSimple(SpellHelp, Canvas->SizeX - SpellHelp->GetSurfaceWidth(), Canvas->SizeY - SpellHelp->GetSurfaceHeight());
	
	if (SpellIndex >= 0 && SpellIndex < Spell.Num())
		DrawTextureSimple(Spell[SpellIndex], 0.f, Canvas->SizeY - SpellHelp->GetSurfaceHeight());
}