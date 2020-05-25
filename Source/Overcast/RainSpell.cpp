// Fill out your copyright notice in the Description page of Project Settings.


#include "RainSpell.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ARainSpell::ARainSpell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up components
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("DummyRoot"));

	CloudParticles = CreateDefaultSubobject<UParticleSystemComponent>("CloudParticles");
	CloudParticles->SetupAttachment(RootComponent);

	RainArea = CreateDefaultSubobject<UBoxComponent>("Wet Area");
	RainArea->InitBoxExtent({ 1.f, 60.f, 300.f });
	RainArea->SetupAttachment(RootComponent);

}

// Called every frame
void ARainSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set the wet area scale
	FVector WetExtent = RainArea->GetUnscaledBoxExtent();
	WetExtent.X += CloudSpeed;
	RainArea->SetBoxExtent(WetExtent);

	// Offset wet area and particles
	RainArea->AddRelativeLocation({ CloudSpeed, 0.f, 0.f });
	CloudParticles->AddRelativeLocation({ CloudSpeed, 0.f, 0.f });
}