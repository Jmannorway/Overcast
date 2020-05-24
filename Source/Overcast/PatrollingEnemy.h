// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrollingEnemy.generated.h"

struct FAIMoveRequest;

UENUM(BlueprintType)
enum class EPatrollingEnemyStatus : uint8
{
	Patrolling,
	Hunting,
	Attacking,
	Stunned,
	Wait,
	NUMBER
};

USTRUCT(BlueprintType)
struct FPatrollingEnemyAttackRadii
{
	GENERATED_USTRUCT_BODY()

public:

	// The radius of an attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Attack;

	// The radius which satisfies the owl's strike zone
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Adjustment;

	// The radius in which the owl will have stopped moving
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Acceptance;

	// A functions that ensures that Attack >= Adjustment >= Acceptance
	void Validate();

	FPatrollingEnemyAttackRadii() { ; }
};

USTRUCT(BlueprintType)
struct FPatrollingEnemyTiming
{
	GENERATED_USTRUCT_BODY()

public:

	// How long to stay out of sight to escape
	UPROPERTY(EditAnywhere)
		float Hunting;

	// The length of an attack
	UPROPERTY(EditAnywhere)
		float Attack;

	// How long the owl is stunned
	UPROPERTY(EditAnywhere)
		float Stun;

	// How long the owl will wait before returning to patrol
	UPROPERTY(EditAnywhere)
		float Wait;

	FPatrollingEnemyTiming() { ; }
};

UCLASS()
class OVERCAST_API APatrollingEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APatrollingEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Visualize parameter changes
	virtual void OnConstruction(const FTransform& Transform) override;

	// Reference to the path to patrol
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class APath* PatrolPath;

	//
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float PatrolAcceptanceRadius;

	//
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float PatrolMovementSpeed;

	// How fast to move while hunting a target
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float HuntingMovementSpeed;

	// Index for the starting point on a path
	UPROPERTY(EditAnywhere, Category = "Patrol")
		uint8 PatrolStartingPoint;

	// How far the patrolling enemy can spot the player from
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float VisionLength;

	// Vision height
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float VisionHeight;

	// State timing (Hover over each element for individual explanation)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
		FPatrollingEnemyTiming Time;

	// The radius of an attack
	UPROPERTY(EditAnywhere, Category = "Patrol")
		FPatrollingEnemyAttackRadii AttackRadii;

	// Collision box used for vision
	UPROPERTY()
		class UBoxComponent* VisionBox;

	// Returns the distance to the targeted actor
	UFUNCTION(BlueprintCallable, Category = "Attack")
		float DistanceToTarget(AActor* Target) const;


	// Functions to be called to check if the player has been seen or not
	UFUNCTION()
		void OnVisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnVisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Functions to be called when the owl is hit by rainclouds
	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	// Controller to handle AI movement
	class AAIController* AIController;

	// Keeping track of the current action
	EPatrollingEnemyStatus Status;

	// True if patrolling a valid path with more than one anchor, false is patrolling a point
	bool bIsPatrollingPath;

	// The path anchor that is currently being moved towards
	uint8 CurrentPathAnchor;

	// A reference to the hunted target
	AActor* TargetActor;

	// Variable set by vision box overlap functions
	bool bTargetInView;

	// Multi-purpose timer variable to do timing with
	float StateTimer;

	// Functions to used when moving to any point
	void MoveToTarget(FVector Target);
	void MoveToTarget(AActor* Target);

	// Subfunctions to be called when getting a move request
	void GenerateEnemyMoveRequest(FAIMoveRequest& OutMoveRequest, FVector Location) const;
	void GenerateEnemyMoveRequest(FAIMoveRequest& OutMoveRequest, AActor* Target) const;

	// Set vision box length and offset the location correctly
	void UpdateVision(float NewVisionLength);

	// Set status
	void SetStatus(EPatrollingEnemyStatus NewStatus);

	// Returns the distance to the target that is currently being targetes
	FORCEINLINE float GetDistanceToTarget() const {
		return FVector::Distance(GetActorLocation(), TargetActor->GetActorLocation());
	}

	/*
		A placeholder variables to update owl animations
	*/
	FVector PreviousLocation;
	float LocationDifference;

public:	
	/*
		The accompanying placeholder functions for owl animations
	*/
	UFUNCTION(BlueprintCallable, Category = "Animation")
		float GetLocationDifference() const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		EPatrollingEnemyStatus GetStatus() const;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		float GetAttackTime() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
