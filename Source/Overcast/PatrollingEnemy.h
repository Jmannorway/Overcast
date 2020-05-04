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
	Stunned
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

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float PatrolMovementSpeed;

	// Index for the starting point on a path
	UPROPERTY(EditAnywhere, Category = "Patrol")
		uint8 StartingPoint;

	// How far the patrolling enemy can spot the player from
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float VisionLength;

	// Collision box used for vision
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class UBoxComponent* VisionBox;


	// For how long the enemy can chase the player without seeing them
	UPROPERTY(EditAnywhere, Category = "Hunting")
		int32 HuntingTimeout;

	// How fast to move while hunting a target
	UPROPERTY(EditAnywhere, Category = "Hunting")
		float HuntingMovementSpeed;


	// The radius of an attack
	UPROPERTY(EditAnywhere, Category = "Attack")
		float AttackRadius;

	// The time before the impact of an attack
	UPROPERTY(EditAnywhere, Category = "Attack")
		float AttackLength;

	// Functions to be called to check if the player has been seen or not
	UFUNCTION()
		void OnVisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnVisionBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	// Timers for timeouts and attacks
	int32 HuntingTimer;
	int32 AttackTimer;

	// Functions to used when moving to any point
	void MoveToTarget(FVector Target);
	void MoveToTarget(AActor* Target);

	// Subfunctions to be called when getting a move request
	void GenerateEnemyMoveRequest(FAIMoveRequest& MoveRequest, FVector Location) const;
	void GenerateEnemyMoveRequest(FAIMoveRequest& MoveRequest, AActor* Target) const;

	// Set vision box length and offset the location correctly
	void UpdateVision(float NewVisionLength);

	// Set status
	void SetStatus(EPatrollingEnemyStatus NewStatus);

	// Returns the distance to the target that is currently being targetes
	FORCEINLINE float GetDistanceToTarget() const;

	/*
		A placeholder variables to update owl animations
	*/
	FVector PreviousLocation;
	float LocationDifference;

public:	
	/*
		The accompanying placeholder functions for owl animations
	*/
	UFUNCTION(BlueprintCallable, Category = "Patrol")
		float GetLocationDifference() const;

	UFUNCTION(BlueprintCallable, Category = "Patrol")
		EPatrollingEnemyStatus GetStatus() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
