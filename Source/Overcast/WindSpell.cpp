// Fill out your copyright notice in the Description page of Project Settings.


#include "WindSpell.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AWindSpell::AWindSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Scene Component"));

	WindMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	WindMesh->SetupAttachment(RootComponent);

	WindArea = CreateDefaultSubobject<UBoxComponent>("Wind hitbox");
	WindArea->InitBoxExtent({ 35.f, 150.f, 150.f });
	WindArea->SetupAttachment(RootComponent);
}

// Called every frame
void AWindSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector WindNL = GetActorLocation();
	WindNL += GetActorForwardVector() * WindSpeed * DeltaTime; 
	SetActorLocation(WindNL);
	

}

