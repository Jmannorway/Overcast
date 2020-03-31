// Fill out your copyright notice in the Description page of Project Settings.

#include "Path.h"
#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Engine/TargetPoint.h"

// Sets default values
APath::APath()
{
	PrimaryActorTick.bCanEverTick = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	DirectionalArrow = CreateDefaultSubobject<UArrowComponent>("Directional Arrow");
	DirectionalArrow->SetVisibility(false);

	PathColor = FColor::Magenta;
}

void APath::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	VisualizePath(PathColor);
}

// Called when the game starts or when spawned
void APath::BeginPlay()
{
	Super::BeginPlay();
	
}

ATargetPoint* APath::GetAnchor(uint32 Index) const
{
	return Anchor[Index];
}

uint32 APath::GetAnchorNumber() const
{
	return Anchor.Num();
}

FVector APath::GetAnchorLocation(uint32 Index) const
{
	return Anchor[Index]->GetActorLocation();
}

FVector APath::GetAnchorLocation(uint32 Index, float Offset) const
{
	return FMath::Lerp(Anchor[Index]->GetActorLocation(), Anchor[(Index + 1) % GetAnchorNumber()]->GetActorLocation(), Offset);
}

void APath::GetAllAnchorLocations(TArray<FVector>& OutArray) const
{
	uint32 PotentialAnchorNumber = Anchor.Num();
	uint32 ValidAnchorNumber = 0;

	OutArray.Init(FVector::ZeroVector, PotentialAnchorNumber);

	while (ValidAnchorNumber < PotentialAnchorNumber && Anchor[ValidAnchorNumber])
	{
		OutArray[ValidAnchorNumber] = Anchor[ValidAnchorNumber]->GetActorLocation();
		ValidAnchorNumber++;
	}

	OutArray.SetNum(ValidAnchorNumber);
}

uint32 APath::GetPointNumber() const
{
	return Point.Num();
}

FVector APath::GetPointLocation(uint32 Index) const
{
	return GetAnchorLocation(Point[Index].Index, Point[Index].Offset);
}

void APath::VisualizePath(const FColor& Color, float Offset, EPathOffsetDirection OffsetDirection, float ArrowOffsetMultiplier) const
{
	UWorld* World = GetWorld();

	FlushPersistentDebugLines(World);

	// Fill the anchor locations array with anchor locations
	TArray<FVector> AnchorLocations;
	GetAllAnchorLocations(AnchorLocations);

	// Set the anchor visualizer offset vector
	FVector AnchorOffsetVector;

	switch (OffsetDirection)
	{
	case EPathOffsetDirection::Right: AnchorOffsetVector = Offset * FVector::RightVector; break;
	case EPathOffsetDirection::Left: AnchorOffsetVector = Offset * FVector::LeftVector; break;
	case EPathOffsetDirection::Forward: AnchorOffsetVector = Offset * FVector::ForwardVector; break;
	case EPathOffsetDirection::Backward: AnchorOffsetVector = Offset * FVector::BackwardVector; break;
	case EPathOffsetDirection::Up: AnchorOffsetVector = Offset * FVector::UpVector; break;
	case EPathOffsetDirection::Down: AnchorOffsetVector = Offset * FVector::DownVector; break;
	}

	// Get the number of valid anchor points
	uint32 ValidAnchorNumber = AnchorLocations.Num();

	// Offset anchor locations for drawing
	for (uint32 i = 0; i < ValidAnchorNumber; i++)
		AnchorLocations[i] += AnchorOffsetVector;

	// Make sure that at least the two first anchor points are valid before drawing the path
	if (ValidAnchorNumber > 1)
	{
		// Configure arrow
		DirectionalArrow->SetVisibility(true);
		DirectionalArrow->SetWorldLocation(AnchorLocations[0] + AnchorOffsetVector * ArrowOffsetMultiplier);
		DirectionalArrow->SetWorldRotation((AnchorLocations[1] - AnchorLocations[0]).Rotation());
		DirectionalArrow->SetArrowColor(Color);

		// Draw lines between anchors
		for (uint32 i = 1; i < ValidAnchorNumber; i++)
			DrawDebugLine(World, AnchorLocations[i - 1], AnchorLocations[i], Color, true);

		// Draw line between first and last anchor unless the last anchor is the second
		if (ValidAnchorNumber > 2)
			DrawDebugLine(World, AnchorLocations[0], AnchorLocations[ValidAnchorNumber - 1], Color, true);

		// Draw a sphere indicating the starting point on the path
		for (int i = Point.Num() - 1; i >= 0; i--)
			DrawDebugSphere(
				World,
				FMath::Lerp(AnchorLocations[Point[i].Index % ValidAnchorNumber], AnchorLocations[(Point[i].Index + 1) % ValidAnchorNumber], FMath::Frac(Point[i].Offset)),
				16.f,
				4,
				Color,
				true
			);
	}
	else
	{
		DirectionalArrow->SetVisibility(false);
	}
}

// Called every frame
void APath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

