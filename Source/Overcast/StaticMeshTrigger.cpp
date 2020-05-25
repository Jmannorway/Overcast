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

	TriggerMovementStep = TriggerMovement / TriggerTime;
	
	TriggerRotationStep.Roll = TriggerRotation.Roll / TriggerTime;
	TriggerRotationStep.Yaw = TriggerRotation.Yaw / TriggerTime;
	TriggerRotationStep.Pitch = TriggerRotation.Pitch / TriggerTime;
}

// Called every frame
void AStaticMeshTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bTriggered && TriggerTime > 0)
	{
		TriggerTime--;
		AddActorLocalOffset(TriggerMovementStep);
		AddActorLocalRotation(TriggerRotationStep);
		UE_LOG(LogTemp, Warning, TEXT("TriggerTime: %i"), TriggerTime);
	}
}

