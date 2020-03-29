// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StaticMeshComponent.h"
#include "PathComponent.h"
#include "EnemyPatrol.h"

// Sets default values
AEnemyPatrol::AEnemyPatrol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>("CharacterMesh");
	SetRootComponent(CharacterMesh);

	Path = CreateDefaultSubobject<UPathComponent>("Path");

	PathColor = FColor::Magenta;
}

// Called when the game starts or when spawned
void AEnemyPatrol::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyPatrol::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Path->VisualizePath(PathColor);
}

// Called every frame
void AEnemyPatrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

