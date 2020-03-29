// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Engine/TargetPoint.h"
#include "PathComponent.h"

#define __VISUALIZER_DEFAULT_OFFSET 128.f
#define __VISUALIZER_DEFAULT_DIRECTION EPathVisualizerDirection::Up
#define __VISUALIZER_DEFAULT_COLOR FColor::Magenta

// Sets default values for this component's properties
UPathComponent::UPathComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Create an arrow to show path direction
	VisualizerArrow = CreateDefaultSubobject<UArrowComponent>("Visualizer Arrow");
	VisualizerArrow->SetVisibility(false);
}


// Called when the game starts
void UPathComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

FVector UPathComponent::GetPathAnchor(uint32 Index) const
{
	return Anchors[Index]->GetActorLocation();
}

uint32 UPathComponent::GetPathAnchorNumber() const
{
	return Anchors.Num();
}

void UPathComponent::VisualizePath()
{
	VisualizePath(__VISUALIZER_DEFAULT_OFFSET, __VISUALIZER_DEFAULT_DIRECTION, __VISUALIZER_DEFAULT_COLOR, 0, 0.f);
}

void UPathComponent::VisualizePath(const FColor& Color)
{
	VisualizePath(__VISUALIZER_DEFAULT_OFFSET, __VISUALIZER_DEFAULT_DIRECTION, Color, 0, 0.f);
}

void UPathComponent::VisualizePath(const FColor& Color, uint32 StartAnchor, float StartAnchorOffset)
{
	VisualizePath(__VISUALIZER_DEFAULT_OFFSET, __VISUALIZER_DEFAULT_DIRECTION, Color, StartAnchor, StartAnchorOffset);
}

void UPathComponent::VisualizePath(float Offset, EPathVisualizerDirection OffsetDirection, const FColor& Color, uint32 StartAnchor, float StartAnchorOffset, float ArrowOffsetMultiplier)
{
	UWorld* World = GetWorld();

	FlushPersistentDebugLines(World);

	// Set the anchor visualizer offset vector
	FVector AnchorOffsetVector;

	switch (OffsetDirection)
	{
	case EPathVisualizerDirection::Right: AnchorOffsetVector = { Offset, 0.f, 0.f }; break;
	case EPathVisualizerDirection::Left: AnchorOffsetVector = { -Offset, 0.f, 0.f }; break;
	case EPathVisualizerDirection::Forward: AnchorOffsetVector = { 0.f, Offset, 0.f }; break;
	case EPathVisualizerDirection::Back: AnchorOffsetVector = { 0.f, -Offset, 0.f }; break;
	case EPathVisualizerDirection::Up: AnchorOffsetVector = { 0.f, 0.f, Offset }; break;
	case EPathVisualizerDirection::Down: AnchorOffsetVector = { 0.f, 0.f, -Offset }; break;
	}

	// Create an array that is at least as big as the potential number of valid anchors
	uint32 PotentialAnchorNumber = Anchors.Num();

	TArray<FVector> AnchorLocations;
	AnchorLocations.Init(FVector::ZeroVector, PotentialAnchorNumber);

	// Get the number of valid anchors and store their offset locations for visualization
	uint32 ValidAnchorNumber = 0;

	while (ValidAnchorNumber < PotentialAnchorNumber && Anchors[ValidAnchorNumber])
	{
		AnchorLocations[ValidAnchorNumber] = Anchors[ValidAnchorNumber]->GetActorLocation() + AnchorOffsetVector;
		ValidAnchorNumber++;
	}

	// Make sure that at least the two first anchor points are valid before drawing the visualizer
	if (ValidAnchorNumber > 1)
	{
		// Configure arrow
		VisualizerArrow->SetVisibility(true);
		VisualizerArrow->SetWorldLocation(AnchorLocations[0] + AnchorOffsetVector * ArrowOffsetMultiplier);
		VisualizerArrow->SetWorldRotation((AnchorLocations[1] - AnchorLocations[0]).Rotation());
		VisualizerArrow->SetArrowColor(Color);

		// Connect anchor points
		for (uint32 i = 1; i < ValidAnchorNumber; i++)
			DrawDebugLine(World, AnchorLocations[i - 1], AnchorLocations[i], Color, true);

		// Draw line between first and last anchor unless the last anchor is the second
		if (ValidAnchorNumber > 2)
			DrawDebugLine(World, AnchorLocations[0], AnchorLocations[ValidAnchorNumber - 1], Color, true);

		// Draw a sphere indicating the starting point on the path
		uint32 NextAnchor = (StartAnchor + 1) % PotentialAnchorNumber;

		if (StartAnchor < ValidAnchorNumber && NextAnchor < ValidAnchorNumber)
			DrawDebugSphere(World, FMath::Lerp(AnchorLocations[StartAnchor], AnchorLocations[NextAnchor], StartAnchorOffset), 24.f, 3, Color, true);
	}
	else
	{
		VisualizerArrow->SetVisibility(false);
	}
}

// Called every frame
void UPathComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

