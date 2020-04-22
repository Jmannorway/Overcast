// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBox.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

APushableBox::APushableBox()
{
	// Active tick
	PrimaryActorTick.bCanEverTick = true;

	// Make the block detectable & movable
	UStaticMeshComponent* Mesh = GetStaticMeshComponent();
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetMobility(EComponentMobility::Movable);

	bIsFalling = false;
	Gravity = 10.f;
}

void APushableBox::AddMovement(const FVector& Movement)
{
	NextMovement += Movement * GetWorld()->GetDeltaSeconds();
}

void APushableBox::Tick(float DeltaTime)
{
	FHitResult Hit;
	AddActorLocalOffset(FVector(0.f, 0.f, -Gravity), true, &Hit);
	bIsFalling = !Hit.IsValidBlockingHit();

	AddActorLocalOffset(NextMovement, true);

	NextMovement = FVector::ZeroVector;
}
