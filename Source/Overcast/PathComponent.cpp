// Fill out your copyright notice in the Description page of Project Settings.

#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Engine/TargetPoint.h"
#include "PathComponent.h"

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

void UPathComponent::VisualizePath()
{
	VisualizePath(128.f, EPathVisualizerDirection::Up, FColor::Magenta);
}

void UPathComponent::VisualizePath(const FColor& Color)
{
	VisualizePath(128.f, EPathVisualizerDirection::Up, Color);
}

void UPathComponent::VisualizePath(float Offset, EPathVisualizerDirection OffsetDirection, const FColor& Color, float ArrowOffsetMultiplier)
{
	float AnchorNumber = Anchors.Num();
	UWorld* World = GetWorld();

	FlushPersistentDebugLines(World);

	if (AnchorNumber > 1 && Anchors[0] && Anchors[1])
	{
		// Set offset vector
		FVector OffsetVector;

		switch (OffsetDirection)
		{
		case EPathVisualizerDirection::Right: OffsetVector = { Offset, 0.f, 0.f }; break;
		case EPathVisualizerDirection::Left: OffsetVector = { -Offset, 0.f, 0.f }; break;
		case EPathVisualizerDirection::Forward: OffsetVector = { 0.f, Offset, 0.f }; break;
		case EPathVisualizerDirection::Back: OffsetVector = { 0.f, -Offset, 0.f }; break;
		case EPathVisualizerDirection::Up: OffsetVector = { 0.f, 0.f, Offset }; break;
		case EPathVisualizerDirection::Down: OffsetVector = { 0.f, 0.f, -Offset }; break;
		}

		FVector FirstAnchorLocation = Anchors[0]->GetActorLocation() + OffsetVector;
		FVector SecondAnchorLocation = Anchors[1]->GetActorLocation() + OffsetVector;

		// Set the position and rotation of the visualizer arrow
		VisualizerArrow->SetVisibility(true);
		VisualizerArrow->SetWorldLocation(FirstAnchorLocation + OffsetVector * ArrowOffsetMultiplier);
		VisualizerArrow->SetWorldRotation((SecondAnchorLocation - FirstAnchorLocation).Rotation());
		VisualizerArrow->SetArrowColor(Color);

		// Draw a line between the two first anchors or a line between the first and the last anchor
		DrawDebugLine(World, FirstAnchorLocation, SecondAnchorLocation, Color, true);

		// Make sure not to overlap draw lines between first and second, and first and last
		if (AnchorNumber > 2 && Anchors[AnchorNumber - 1])
			DrawDebugLine(World, FirstAnchorLocation, Anchors[AnchorNumber - 1]->GetActorLocation() + OffsetVector, Color, true);

		// Draw lines connecting anchors from the second onwards
		for (int32 i = 2; i < AnchorNumber && Anchors[i]; i++)
			DrawDebugLine(World, Anchors[i - 1]->GetActorLocation() + OffsetVector, Anchors[i]->GetActorLocation() + OffsetVector, Color, true);
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

