// Fill out your copyright notice in the Description page of Project Settings.


#include "Pushableblock.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
APushableblock::APushableblock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Scene Component"));


	UStaticMeshComponent* BlockMesh = GetStaticMeshComponent();
	BlockMesh->SetupAttachment(RootComponent);
	BlockMesh->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	BlockMesh->SetMobility(EComponentMobility::Movable);

	BlockBox = CreateDefaultSubobject<UBoxComponent>("Block Hitbox");

}

