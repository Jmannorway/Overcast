// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class OVERCAST_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameHUD();

	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int32 SpellIndex;
	
protected:
	UPROPERTY(EditAnywhere, Category = "HUD")
		UTexture* SpellHelp;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
		TArray<UTexture*> Spell;
};
