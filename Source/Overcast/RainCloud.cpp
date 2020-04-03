// Fill out your copyright notice in the Description page of Project Settings.


#include "RainCloud.h"
#include "Engine/TargetPoint.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

#define DEFAULT_WETAREA_WIDTH 60.f
#define DEFAULT_WETAREA_DEPTH 300.f

// Sets default values
ARainCloud::ARainCloud()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Defaults
	CloudLife = 200;
	CloudSpeed = 15.f;

	// Set up components
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DummyRoot"));

	CloudParticles = CreateDefaultSubobject<UParticleSystemComponent>("CloudParticles");
	CloudParticles->SetupAttachment(RootComponent);

	WetArea = CreateDefaultSubobject<UBoxComponent>("Wet Area");
	WetArea->InitBoxExtent({ 1.f, DEFAULT_WETAREA_WIDTH, DEFAULT_WETAREA_DEPTH });
	WetArea->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ARainCloud::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARainCloud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Destroy actor when life runs out
	if (CloudLife > CloudLifeCount)
		CloudLifeCount++;
	else
		Destroy();
		
	// Set the wet area scale
	FVector WetExtent = WetArea->GetUnscaledBoxExtent();
	WetExtent.X += CloudSpeed;
	WetArea->SetBoxExtent(WetExtent);
	
	// Offset wet area and particles
	WetArea->AddRelativeLocation({ CloudSpeed, 0.f, 0.f });
	CloudParticles->AddRelativeLocation({ CloudSpeed, 0.f, 0.f });
}

