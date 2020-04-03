// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PatrollingEnemy.generated.h"

UENUM(BlueprintType)
enum class EPatrollingEnemyStatus : uint8
{
	Patrolling,
	Hunting,
	Attacking,
	ERROR
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
		class APath* Path;

	// Index for the starting point on a path
	UPROPERTY(EditAnywhere, Category = "Patrol")
		uint8 StartingPoint;

	// How far the patrolling enemy can spot the player from
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float VisionLength;

	// Collision box used for vision
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class UBoxComponent* VisionBox;

	// The radius of an attack
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float AttackRadius;

	// How far into the target has to be into the attack radius to be attacked
	UPROPERTY(EditAnywhere, Category = "Patrol")
		float AttackRadiusUsage;

	// Function to be called when the player overlaps the vision box
	UFUNCTION()
		void OnVisionBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	// Controller to move this character to various targets
	class AAIController* AIController;

	// Keeping track of the current action
	EPatrollingEnemyStatus Status;

	// True if patrolling a valid path with more than one anchor, false is patrolling a point
	bool bIsPatrollingPath;

	// The path anchor that is currently being moved towards
	uint8 CurrentPathAnchor;

	// Function to used when moving to any point
	void MoveToTarget(FVector Target);
	void MoveToTarget(AActor* Target);

	// Set vision box length and offset the location correctly
	void SetVision(float NewVisionLength);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
