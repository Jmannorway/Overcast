// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

class UUserWidget;

/**
 *
 */
UCLASS()
class OVERCAST_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMenuGameMode();

	UFUNCTION(BlueprintCallable, Category = "Menu")
		void SetWBP(TSubclassOf<UUserWidget> NewWidget);

	UFUNCTION(BlueprintCallable, Category = "Menu")
		void RemoveWBP();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Menu")
		TSubclassOf<UUserWidget> StartingWidget;

	UUserWidget* CurrentWidget;

};
