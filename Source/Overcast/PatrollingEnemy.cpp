// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollingEnemy.h"
#include "DrawDebugHelpers.h"
#include "Path.h"
#include "AIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

#define AI_ACCEPTANCE_RADIUS 24.f

// Sets default values
APatrollingEnemy::APatrollingEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default status
	Status = EPatrollingEnemyStatus::Patrolling;

	// Init vision box component
	VisionBox = CreateDefaultSubobject<UBoxComponent>("Vision Box");
	VisionBox->SetupAttachment(RootComponent);

	// Set size and relative location based on root component if it is a capsule
	UCapsuleComponent* RootCapsule = Cast<UCapsuleComponent>(RootComponent);

	if (RootCapsule)
	{
		VisionBox->SetRelativeLocation({ RootCapsule->GetScaledCapsuleRadius(), 0.f, 0.f });
		VisionBox->SetBoxExtent({ 0.f, RootCapsule->GetScaledCapsuleRadius() * 2.f, RootCapsule->GetScaledCapsuleHalfHeight() });
	}
}

// Called when the game starts or when spawned
void APatrollingEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Validate aicontroller, path, and path anchor number before doing anything
	AIController = Cast<AAIController>(GetController());

	if (!AIController || !Path || Path->GetAnchorNumber() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy Patrol: Invalid aicontroller / invalid path / path doesn't have any anchors"));
		Destroy();
		return;
	}

	// Set whether or not patrolling a point or a path
	bIsPatrollingPath = Path->GetAnchorNumber() > 1;

	// Move to starting location if not already there
	FVector AnchorLocation;

	if (bIsPatrollingPath && StartingPoint < Path->GetPointNumber()) // starting location is a defined point on the path
	{
		AnchorLocation = Path->GetPointLocation(StartingPoint);
		CurrentPathAnchor = Path->GetPointAnchor(StartingPoint);
	}
	else // starting location is the first path anchor
		AnchorLocation = Path->GetAnchorLocation(0);

	// Move to starting point if not already there
	if (FVector::Distance(GetActorLocation(), AnchorLocation) > AI_ACCEPTANCE_RADIUS)
		MoveToLocation(AnchorLocation);

	// Set vision box length correctly
	UpdateVision();

}

void APatrollingEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateVision();
}

void APatrollingEnemy::MoveToLocation(FVector Location)
{
	FAIMoveRequest Request;
	Request.SetGoalLocation(Location);
	Request.SetAcceptanceRadius(24.f);

	AIController->MoveTo(Request);
}

void APatrollingEnemy::UpdateVision()
{
	// Visualize changes in vision length
	FVector VisionBoxExtent = VisionBox->GetScaledBoxExtent();
	FVector VisionBoxLocation = VisionBox->GetRelativeLocation();
	FVector VisionBoxRotation = GetActorRotation().Vector();

	VisionBoxRotation.Normalize();

	VisionBox->SetBoxExtent({ VisionLength, VisionBoxExtent.Y, VisionBoxExtent.Z });
	VisionBox->SetRelativeRotation(GetActorRotation());
	VisionBox->SetRelativeLocation({ VisionBoxRotation * VisionLength });
}

// Called every frame
void APatrollingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the vision box to face in the direction that the character is facing
	UpdateVision();
	DrawDebugBox(GetWorld(), GetActorLocation() + VisionBox->GetRelativeLocation(), VisionBox->GetScaledBoxExtent(), (FQuat)GetActorRotation(), FColor::White);

	switch (Status)
	{
	case EPatrollingEnemyStatus::Patrolling:
		// Start moving towards the next point if not moving
		if (bIsPatrollingPath && AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			CurrentPathAnchor = Path->GetNextAnchorIndex(CurrentPathAnchor);
			MoveToLocation(Path->GetAnchorLocation(CurrentPathAnchor));
		}
		break;

	case EPatrollingEnemyStatus::Hunting:
		break;

	case EPatrollingEnemyStatus::Attacking:
		break;
	}
}

// Called to bind functionality to input
void APatrollingEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

