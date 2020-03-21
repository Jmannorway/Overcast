// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPatrol.h"

// Sets default values
AEnemyPatrol::AEnemyPatrol()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyPatrol::BeginPlay()
{
	Super::BeginPlay();
	
	int32 AnchorNumber = Anchor.Num();

	// Safely set various anchor variables
	StartAnchor = FMath::Clamp(StartAnchor, 0, AnchorNumber);
	StartAnchorOffset = FMath::Clamp(StartAnchorOffset, 0.f, 1.f);
	NextAnchor = GetNextAnchor(StartAnchor);

	// Move the actor to the starting anchor + offset
	SetActorLocation(Anchor[StartAnchor] + (Anchor[NextAnchor] - Anchor[StartAnchor]) * StartAnchorOffset);
}

void AEnemyPatrol::SetMovementState(EEP_MovementState NewState)
{
	State = NewState;
}

int32 AEnemyPatrol::GetNextAnchor(int32 AnchorIndex) const
{
	return AnchorIndex + 1 < Anchor.Num() ? AnchorIndex + 1 : 0;
}

// Called every frame
void AEnemyPatrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyPatrol::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

