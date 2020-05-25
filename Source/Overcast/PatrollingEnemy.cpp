// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollingEnemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Path.h"
#include "Player1.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OvercastMainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "RainSpell.h"

// Sets default values
APatrollingEnemy::APatrollingEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default patrol variables
	PatrolMovementSpeed = 240.f;
	VisionLength = 400.f;
	PatrolAcceptanceRadius = 24.f;

	// Default hunting variables
	HuntingMovementSpeed = 450.f;

	// Default attack variables
	AttackRadii.Attack = 128.f;
	AttackRadii.Adjustment = 96.f;
	AttackRadii.Acceptance = 64.f;

	// Default timing
	Time.Hunting = 4.f;
	Time.Attack = 0.5f;
	Time.Stun = 2.f;
	Time.Wait = 3.5f;

	SetStatus(EPatrollingEnemyStatus::Patrolling);

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

	if (bIsPatrollingPath && PatrolStartingPoint < PatrolPath->GetPointNumber()) // starting location is a defined point on the path
	{
		AnchorLocation = PatrolPath->GetPointLocation(PatrolStartingPoint);
		CurrentPathAnchor = PatrolPath->GetPointAnchor(PatrolStartingPoint);
	}
	else // starting location is the first path anchor
		AnchorLocation = PatrolPath->GetAnchorLocation(0);

	// Move to starting point if not already there
	if (FVector::Distance(GetActorLocation(), AnchorLocation) > PatrolAcceptanceRadius)
		MoveToTarget(AnchorLocation);

	// Bind delegate with the vision collision box
	VisionBox->OnComponentBeginOverlap.AddDynamic(this, &APatrollingEnemy::OnVisionBoxBeginOverlap);
	VisionBox->OnComponentEndOverlap.AddDynamic(this, &APatrollingEnemy::OnVisionBoxEndOverlap);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APatrollingEnemy::OnCapsuleBeginOverlap);

	// Set vision box length correctly
	UpdateVision(VisionLength);

}

void APatrollingEnemy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UpdateVision(VisionLength);

	// Make sure attack radii values are valid
	AttackRadii.Validate();
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

void APatrollingEnemy::GenerateEnemyMoveRequest(FAIMoveRequest& OutMoveRequest, FVector Location) const
{
	OutMoveRequest.SetAcceptanceRadius(PatrolAcceptanceRadius);
	OutMoveRequest.SetGoalLocation(Location);
}

void APatrollingEnemy::GenerateEnemyMoveRequest(FAIMoveRequest& OutMoveRequest, AActor* Target) const
{
	OutMoveRequest.SetAcceptanceRadius(AttackRadii.Acceptance);
	OutMoveRequest.SetGoalActor(Target);
}

float APatrollingEnemy::DistanceToTarget(AActor* Target) const
{
	return FVector::Distance(GetActorLocation(), Target->GetActorLocation());
}

void APatrollingEnemy::OnVisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Status != EPatrollingEnemyStatus::Stunned)
	{
		APlayer1* Player = Cast<APlayer1>(OtherActor);

		if (Player)
		{
			// Get information about the fastest AI path to the player
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

					GetCharacterMovement()->MaxWalkSpeed = HuntingMovementSpeed;
				}

				StateTimer = 0.f;
				bTargetInView = true;
				TargetActor = OtherActor;
			}
		}
	}
}

void APatrollingEnemy::OnVisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bTargetInView = false;
}

void APatrollingEnemy::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ARainSpell>(OtherActor))
		SetStatus(EPatrollingEnemyStatus::Stunned);
}

void APatrollingEnemy::UpdateVision(float NewVisionLength)
{
	// Visualize changes in vision length
	FVector VisionBoxExtent = VisionBox->GetScaledBoxExtent();
	VisionBox->SetBoxExtent({ VisionLength, VisionBoxExtent.Y, VisionHeight });
	VisionBox->SetRelativeLocation({ VisionLength, 0.f, VisionHeight - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() });
}

void APatrollingEnemy::SetStatus(EPatrollingEnemyStatus NewStatus)
{
	// Transition from status...
	if (Status == EPatrollingEnemyStatus::Attacking)
		GetCharacterMovement()->bOrientRotationToMovement = true;

	// ...to status
	switch (NewStatus)
	{
	case EPatrollingEnemyStatus::Patrolling:
		GetCharacterMovement()->MaxWalkSpeed = PatrolMovementSpeed;
		TargetActor = nullptr;
		break;

	case EPatrollingEnemyStatus::Hunting:
		GetCharacterMovement()->MaxWalkSpeed = HuntingMovementSpeed;
		break;

	case EPatrollingEnemyStatus::Attacking:
		GetCharacterMovement()->MaxWalkSpeed = HuntingMovementSpeed;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		break;

	case EPatrollingEnemyStatus::Stunned:
	case EPatrollingEnemyStatus::Wait:
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		TargetActor = nullptr;
		break;
	}

	// Set status changing variables
	StateTimer = 0.f;
	Status = NewStatus;
}

float APatrollingEnemy::GetLocationDifference() const
{
	return LocationDifference;
}

EPatrollingEnemyStatus APatrollingEnemy::GetStatus() const
{
	return Status;
}

float APatrollingEnemy::GetAttackTime() const
{
	return Time.Attack;
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

		// Increment action timer if target is in view
		StateTimer = bTargetInView ? 0 : StateTimer + DeltaTime;
		UE_LOG(LogTemp, Warning, TEXT("StateTimer = %f"), StateTimer);

		// Return to patrolling if target actor is invalid or escaped
		if (StateTimer > Time.Hunting || !TargetActor || TargetActor->IsPendingKill())
		{
			SetStatus(EPatrollingEnemyStatus::Patrolling);
			AIController->StopMovement();
			bTargetInView = false;
			UE_LOG(LogTemp, Warning, TEXT("Enemy Patrol: You escaped!"));
		} // Start attacking if within range
		else if (DistanceToTarget(TargetActor) < AttackRadii.Attack)
		{
			SetStatus(EPatrollingEnemyStatus::Attacking);
			UE_LOG(LogTemp, Warning, TEXT("Enemy Patrol: You are under attack!"));
		}

		break;

	case EPatrollingEnemyStatus::Attacking:

		if (TargetActor && !TargetActor->IsPendingKill() && DistanceToTarget(TargetActor) <= AttackRadii.Attack)
		{
			// Always face the player
			const float TargetActorYaw = TargetActor->GetActorRotation().Pitch;
			const float PatrollingEnemyYaw = GetActorRotation().Pitch;
			AddActorLocalRotation(FRotator(0.f, TargetActorYaw, 0.f) - FRotator(0.f, PatrollingEnemyYaw, 0.f));

			StateTimer += DeltaTime;

			// The attack landed, load the game
			if (StateTimer >= Time.Attack)
			{
				if (auto GameMode = Cast<AOvercastMainGameMode>(UGameplayStatics::GetGameMode(this)))
				{
					UE_LOG(LogTemp, Warning, TEXT("You are dead boi"));
					GameMode->LoadGame();
					SetStatus(EPatrollingEnemyStatus::Patrolling);
				}
			}
			else if (AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
			{
				// Follow the player to the attack acceptance radius once out of adjustment radius
				if (DistanceToTarget(TargetActor) > AttackRadii.Adjustment)
				{
					MoveToTarget(TargetActor);
				}
			}
		}
		else
		{
			if (bTargetInView)
				SetStatus(EPatrollingEnemyStatus::Hunting);
			else
				SetStatus(EPatrollingEnemyStatus::Wait);
		}
		break;

	case EPatrollingEnemyStatus::Stunned:
		StateTimer += DeltaTime;
		if (StateTimer >= Time.Stun)
			SetStatus(EPatrollingEnemyStatus::Hunting);
		break;

	case EPatrollingEnemyStatus::Wait:
		StateTimer += DeltaTime;
		if (StateTimer >= Time.Wait)
			SetStatus(EPatrollingEnemyStatus::Patrolling);
		break;
	}

	// Update location difference for function calls
	FVector Location = GetActorLocation();

	LocationDifference = FMath::Abs(PreviousLocation.X - Location.X) + FMath::Abs(PreviousLocation.Y - Location.Y);
	PreviousLocation = Location;
}

void FPatrollingEnemyAttackRadii::Validate()
{
	Adjustment = FMath::Min(Attack, Adjustment);
	Acceptance = FMath::Min(Adjustment, Acceptance);
}

// Old attack state code
/*
if (TargetActor && !TargetActor->IsPendingKill() && GetDistanceToTarget() < AttackRadii.Attack)
		{
			StateTimer++;

			// Make the owl face the player
			const float TargetActorYaw = TargetActor->GetActorRotation().Pitch;
			const float PatrollingEnemyYaw = GetActorRotation().Pitch;
			AddActorLocalRotation(FRotator(0.f, TargetActorYaw, 0.f) - FRotator(0.f, PatrollingEnemyYaw, 0.f));

			if (StateTimer > AttackTime)
			{
				Cast<AOvercastMainGameMode>(UGameplayStatics::GetGameMode(this))->LoadGame();
				SetStatus(EPatrollingEnemyStatus::Wait);

				UE_LOG(LogTemp, Warning, TEXT("Attacking -> Patrolling & DEAD AF"));
			}
		}
		else
		{
			SetStatus(EPatrollingEnemyStatus::Hunting);
			UE_LOG(LogTemp, Warning, TEXT("Attacking -> Hunting"));
		}
*/
