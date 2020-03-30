// Fill out your copyright notice in the Description page of Project Settings.

#include "RainCloud.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

// Set to false when you want the wet area box to be invisible
#define __RAIN_AREA_DEBUG true

// Sets default values
ARainCloud::ARainCloud()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CloudParticles = CreateDefaultSubobject<UParticleSystemComponent>("Cloud Particles");
	SetRootComponent(CloudParticles);

	RainParticles = CreateDefaultSubobject<UParticleSystemComponent>("Rain Particles");
	RainParticles->SetupAttachment(RootComponent);

	RainArea = CreateDefaultSubobject<UBoxComponent>("Rain Area");
	RainArea->SetupAttachment(RootComponent);
	RainArea->SetUsingAbsoluteLocation(true);

	// Default life time
	CloudLife = 150.f;
	RainLife = 75.f;

	// Default rain area parameters
	RainReachDepth = 300.f;
	RainReachWidth = 60.f;

	// Default movement variables
	CloudSpeed = 15.f;
}

// Called when the game starts or when spawned
void ARainCloud::BeginPlay()
{
	Super::BeginPlay();

	// Configure rain area box
	RainArea->AddLocalOffset(GetActorLocation());
	RainArea->SetBoxExtent(FVector(0.f, RainReachWidth, RainReachDepth));
	RainArea->AddLocalRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
}

// Called every frame
void ARainCloud::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector FrameSpeed = GetActorRotation().Vector() * CloudSpeed;

	// Move the cloud in the set direction
	SetActorLocation(GetActorLocation() + FrameSpeed);

	// Destroy when the time is over
	if (CloudLife == 0)
		Destroy();
	else
		CloudLife--;

	// Set rain area size
	float FrameSpeedSize = FrameSpeed.Size();

	RainArea->SetBoxExtent(FVector(RainArea->GetUnscaledBoxExtent().X + FrameSpeed.Size(), RainReachWidth, RainReachDepth));
	RainArea->AddLocalOffset(FVector(FrameSpeedSize, 0.f, 0.f));
	
	// Draw rain area indication
#if __RAIN_AREA_DEBUG
	DrawDebugBox(
		GetWorld(),
		RainArea->GetRelativeLocation(),
		RainArea->GetUnscaledBoxExtent(),
		(FQuat)RainArea->GetRelativeRotation(),
		FColor::Yellow
	);
#endif
}

