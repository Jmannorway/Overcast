// Fill out your copyright notice in the Description page of Project Settings.


#include "StaticMeshTrigger.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AStaticMeshTrigger::AStaticMeshTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a mesh component
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	// Trigger default values
	TriggerMovement = { 0.f, 0.f, -400.f };
	TriggerMovementSpeed = 5.f;
}

void AStaticMeshTrigger::Trigger()
{
	if (!bTriggered)
		bTriggered = true;
}

int32 AStaticMeshTrigger::GetTriggerIndex() const
{
	return TriggerIndex;
}

// Called when the game starts or when spawned
void AStaticMeshTrigger::BeginPlay()
{
	Super::BeginPlay();

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	const FVector Location = GetActorLocation();
	TriggerMovementTime = FMath::Abs(FVector::Distance(Location, Location + TriggerMovement)) / TriggerMovementSpeed;
	TriggerMovementStep = TriggerMovement / TriggerMovementTime;
}

// Called every frame
void AStaticMeshTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTriggered && TriggerMovementTime > 0)
	{
		TriggerMovementTime--;
		AddActorLocalOffset(TriggerMovementStep);
		UE_LOG(LogTemp, Warning, TEXT("TriggerMovementTime: %i"), TriggerMovementTime);
	}
}

