// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/ArrowComponent.h"
#include "Engine/TargetPoint.h"
#include "EnemyPatrol.h"

#ifdef UE_BUILD_DEBUG
#include "DrawDebugHelpers.h"
#endif

// Sets default values
AEnemyPatrol::AEnemyPatrol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Anchors.Reserve(1);

	PathHelperArrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>("Path Helper Arrow");
	PathColorInEditor = FColor::Magenta;
}

void AEnemyPatrol::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#ifdef UE_BUILD_DEBUG

	const int32 AnchorNumber =  Anchors.Num();

	if (AnchorNumber > 1)
	{
		const FVector Location = GetActorLocation();
		const FVector FirstAnchorLocation = Anchors[0] + Location;

		PathHelperArrow->SetVisibility(true);
		PathHelperArrow->SetWorldLocationAndRotation(FirstAnchorLocation, { ((Anchors[1] + Location) - FirstAnchorLocation).Rotation() });
		PathHelperArrow->SetArrowColor(PathColorInEditor);

		const UWorld* World = GetWorld();

		FlushPersistentDebugLines(World);

		for (int32 i = 1; i < AnchorNumber; i++)
		{
			DrawDebugLine(World, Location + Anchors[i - 1], Location + Anchors[i], PathColorInEditor, true);
		}

		DrawDebugLine(World, FirstAnchorLocation, Location + Anchors[AnchorNumber - 1], PathColorInEditor, true);
	}
	else
	{
		FlushPersistentDebugLines(GetWorld());
		PathHelperArrow->SetVisibility(false);
	}
#endif
}

// Called when the game starts or when spawned
void AEnemyPatrol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyPatrol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

