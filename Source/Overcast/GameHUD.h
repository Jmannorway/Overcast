// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */

class APlayer1;

UCLASS()
class OVERCAST_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameHUD();

	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
		void SetHUDSpell(uint8 SpellIndex);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		uint8 LastSpellIndex;
	
protected:

	/*
		HUD Textures
	*/

	UPROPERTY(EditAnywhere, Category = "HUD")
		UTexture* SpellHelpTexture;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TArray<UTexture*> SpellTextures;
};
