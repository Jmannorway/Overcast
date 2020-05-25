// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadeSpell.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AShadeSpell::AShadeSpell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a collision component
	SunCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SunCollision->InitSphereRadius(192.f);
	SetRootComponent(SunCollision);

	// Create a mesh component
	SunMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SunMesh->SetupAttachment(RootComponent);
}

void AShadeSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}