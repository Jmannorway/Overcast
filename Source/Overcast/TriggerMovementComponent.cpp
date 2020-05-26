// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerMovementComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UTriggerMovementComponent::UTriggerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TriggerTime = 60;
}

void UTriggerMovementComponent::Trigger()
{
	if (!bTriggered)
		bTriggered = true;
}


int32 UTriggerMovementComponent::GetTriggerIndex() const
{
	return TriggerIndex;
}

// Called when the game starts
void UTriggerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	TriggerMovementStep = TriggerMovement / TriggerTime;

	TriggerRotationStep.Roll = TriggerRotation.Roll / TriggerTime;
	TriggerRotationStep.Yaw = TriggerRotation.Yaw / TriggerTime;
	TriggerRotationStep.Pitch = TriggerRotation.Pitch / TriggerTime;
}


// Called every frame
void UTriggerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Move the owner when triggered

	if (bTriggered && TriggerTime > 0)
	{
		TriggerTime--;
		GetOwner()->AddActorLocalOffset(TriggerMovementStep);
		GetOwner()->AddActorLocalRotation(TriggerRotationStep);
		UE_LOG(LogTemp, Warning, TEXT("TriggerTime: %i"), TriggerTime);
	}
}

