// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollingEnemy.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Path.h"
#include "Player1.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"

#define AI_DEFAULT_ACCEPTANCE_RADIUS 24.f

// Sets default values
APatrollingEnemy::APatrollingEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default values
	AttackRadius = 72.f;
	VisionLength = 500.f;
	Status = EPatrollingEnemyStatus::Patrolling;
	bTargetInView = false;
	HuntingTimeout = 300;

	// Set movement defaults
	auto Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = 300.f;

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
	// Call overridden function
	Super::BeginPlay();

	// Validate aicontroller, path, and path anchor number before doing anything
	AIController = Cast<AAIController>(GetController());

	if (!AIController || !PatrolPath || PatrolPath->GetAnchorNumber() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Enemy Patrol: Invalid aicontroller / invalid path / path doesn't have any anchors"));
		Destroy();
		return;
	}

	// Set whether or not patrolling a point or a path
	bIsPatrollingPath = PatrolPath->GetAnchorNumber() > 1;

	// Move to starting location if not already there
	FVector AnchorLocation;

	if (bIsPatrollingPath && StartingPoint < PatrolPath->GetPointNumber()) // starting location is a defined point on the path
	{
		AnchorLocation = PatrolPath->GetPointLocation(StartingPoint);
		CurrentPathAnchor = PatrolPath->GetPointAnchor(StartingPoint);
	}
	else // starting location is the first path anchor
		AnchorLocation = PatrolPath->GetAnchorLocation(0);

	// Move to starting point if not already there
	if (FVector::Distance(GetActorLocation(), AnchorLocation) > AI_DEFAULT_ACCEPTANCE_RADIUS)
		MoveToTarget(AnchorLocation);

	// Bind delegate with the vision collision box
	VisionBox->OnComponentBeginOverlap.AddDynamic(this, &APatrollingEnemy::OnVisionBoxBeginOverlap);
	VisionBox->OnComponentEndOverlap.AddDynamic(this, &APatrollingEnemy::OnVisionBoxEndOverlap);

	// Set vision box length correctly
	UpdateVision(VisionLength);

}

void APatrollingEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateVision(VisionLength);

	FlushPersistentDebugLines(GetWorld());
	DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRadius, (int32)AttackRadius / 16, FColor::Yellow, true);
}

void APatrollingEnemy::MoveToTarget(FVector Target)
{
	FAIMoveRequest Request;
	GenerateEnemyMoveRequest(Request, Target);

	AIController->MoveTo(Request);
}

void APatrollingEnemy::MoveToTarget(AActor* Target)
{
	FAIMoveRequest Request;
	GenerateEnemyMoveRequest(Request, Target);

	AIController->MoveTo(Request);
}

void APatrollingEnemy::GenerateEnemyMoveRequest(FAIMoveRequest& MoveRequest, FVector Location) const
{
	MoveRequest.SetAcceptanceRadius(AI_DEFAULT_ACCEPTANCE_RADIUS);
	MoveRequest.SetGoalLocation(Location);
}

void APatrollingEnemy::GenerateEnemyMoveRequest(FAIMoveRequest& MoveRequest, AActor* Target) const
{
	//MoveRequest.SetAcceptanceRadius(AttackRadius);
	MoveRequest.SetGoalActor(Target);
}

void APatrollingEnemy::OnVisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	class APlayer1* Player = Cast<APlayer1>(OtherActor);

	if (Player)
	{
		FAIMoveRequest Request;
		GenerateEnemyMoveRequest(Request, OtherActor);

		FPathFindingQuery Query;
		AIController->BuildPathfindingQuery(Request, Query);

		FNavPathSharedPtr Path;
		AIController->FindPathForMoveRequest(Request, Query, Path);

		// Chase player if nav path points are less than three
		if (Path && Path->GetPathPoints().Num() < 3)
		{
			if (Status == EPatrollingEnemyStatus::Patrolling)
			{
				AIController->MoveTo(Request);
				Status = EPatrollingEnemyStatus::Hunting;
				UE_LOG(LogTemp, Warning, TEXT("Patrolling enemy: status = hunting, chasing player actor"));
				CurrentPathAnchor = PatrolPath->GetPreviousAnchorIndex(CurrentPathAnchor);
			}

			HuntingTimer = 0;
			bTargetInView = true;
			TargetActor = OtherActor;
		}
	}
}

void APatrollingEnemy::OnVisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bTargetInView = false;
}

void APatrollingEnemy::UpdateVision(float NewVisionLength)
{
	// Visualize changes in vision length
	FVector VisionBoxExtent = VisionBox->GetScaledBoxExtent();
	VisionBox->SetBoxExtent({ VisionLength, VisionBoxExtent.Y, VisionBoxExtent.Z });
	VisionBox->SetRelativeLocation({ VisionLength, 0.f, 0.f });
}

float APatrollingEnemy::GetLocationDifference() const
{
	return LocationDifference;
}

// Called every frame
void APatrollingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make sure not to reference a non-exising actor
	if (TargetActor != nullptr && (TargetActor->IsPendingKill() || Status == EPatrollingEnemyStatus::Patrolling))
	{
		TargetActor = nullptr;
		Status = EPatrollingEnemyStatus::Patrolling;
	}

	// Increment the hunting timer
	HuntingTimer += !bTargetInView;

	switch (Status)
	{
	case EPatrollingEnemyStatus::Patrolling:
		// Start moving towards the next point if not moving
		if (bIsPatrollingPath && AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
		{
			CurrentPathAnchor = PatrolPath->GetNextAnchorIndex(CurrentPathAnchor);
			MoveToTarget(PatrolPath->GetAnchorLocation(CurrentPathAnchor));
		}
		break;

	case EPatrollingEnemyStatus::Hunting:

		UE_LOG(LogTemp, Warning, TEXT("HuntingTimer = %i"), HuntingTimer);

		if (FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation()) < AttackRadius)
		{
			Status = EPatrollingEnemyStatus::Attacking;
			AIController->StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("Enemy Patrol: You were hit!"));
			bTargetInView = false;
		}
		else if (HuntingTimer > HuntingTimeout)
		{
			Status = EPatrollingEnemyStatus::Patrolling;
			AIController->StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("Enemy Patrol: You escaped!"));
			bTargetInView = false;
		}

		break;

	case EPatrollingEnemyStatus::Attacking:
		Status = EPatrollingEnemyStatus::Patrolling;
		UE_LOG(LogTemp, Warning, TEXT("Going back to patrolling"));
		break;
	}

	// Update location difference for function calls
	FVector Location = GetActorLocation();

	LocationDifference = FMath::Abs(PreviousLocation.X - Location.X) + FMath::Abs(PreviousLocation.Y - Location.Y);
	PreviousLocation = Location;
}

