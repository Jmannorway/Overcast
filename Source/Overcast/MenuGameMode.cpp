// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameMode.h"
#include "Blueprint/UserWidget.h"

AMenuGameMode::AMenuGameMode()
{

}

void AMenuGameMode::BeginPlay()
{
	if (StartingWidget)
	{
		SetWBP(StartingWidget);
	}
}

void AMenuGameMode::SetWBP(TSubclassOf<UUserWidget> NewWidget)
{
	RemoveWBP();

	if (NewWidget)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidget);
		CurrentWidget->AddToViewport();
	}
}

void AMenuGameMode::RemoveWBP()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
}


