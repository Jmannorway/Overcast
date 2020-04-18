// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraTrigger.h"
#include "Player1.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACameraTrigger::ACameraTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Set up trigger box
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("BoxTrigger");
	SetRootComponent(TriggerBox);
	TriggerBox->InitBoxExtent({ 80.f, 80.f, 80.f });
	TriggerBox->ShapeColor = FColor::Green;

#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Sprite");
	SpriteComponent->SetupAttachment(RootComponent);
#endif
}

ECameraTriggerReaction ACameraTrigger::GetInsideReaction() const
{
	return InsideReaction;
}

ECameraTriggerReaction ACameraTrigger::GetOutsideReaction() const
{
	return OutsideReaction;
}

const FShot& ACameraTrigger::GetInsideShot() const
{
	return InsideShot;
}

const FShot& ACameraTrigger::GetOutsideShot() const
{
	return OutsideShot;
}
