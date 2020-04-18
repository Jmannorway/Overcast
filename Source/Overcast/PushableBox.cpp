// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBox.h"
#include "Components/StaticMeshComponent.h"

APushableBox::APushableBox()
{
	// Active tick
	PrimaryActorTick.bCanEverTick = true;

	// Make the block detectable & movable
	UStaticMeshComponent* Mesh = GetStaticMeshComponent();
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetMobility(EComponentMobility::Movable);
}

void APushableBox::MoveForward(float Speed)
{
	FVector Location = GetActorLocation();
	Location.X += Speed / 60.f;
	SetActorLocation(Location, true);
}

void APushableBox::MoveHorizontally(float Speed)
{
	FVector Location = GetActorLocation();
	Location.Y += Speed / 60.f;
	SetActorLocation(Location, true);
}