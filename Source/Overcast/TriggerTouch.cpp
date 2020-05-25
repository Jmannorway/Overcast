// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerTouch.h"
#include "Components/StaticMeshComponent.h"
#include "TriggerComponent.h"

// Sets default values
ATriggerTouch::ATriggerTouch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	TriggerComponent = CreateDefaultSubobject<UTriggerComponent>("TriggerComponent");

	OnActorBeginOverlap.AddDynamic(this, &ATriggerTouch::Overlap);
}

void ATriggerTouch::Overlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	TriggerAndDestroy();
}

void ATriggerTouch::TriggerAndDestroy()
{
	TriggerComponent->Trigger();
	Destroy();
}

// Called every frame
void ATriggerTouch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate in world
	AddActorLocalRotation({ 0.f, 120.f * DeltaTime, 0.f });
}

