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

	// Visualize changes in vision
	virtual void OnConstruction(const FTransform& Transform) override;

	// Path to patrol
	UPROPERTY(EditAnywhere, Category = "Patrol")
		class APath* Path;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		uint8 StartingPoint;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		float VisionLength;

private:

	// Controller to move this character to various targets
	class AAIController* AIController;

	// Keeping track of the current action
	EPatrollingEnemyStatus Status;

	// True if patrolling a valid path with more than one anchor, false is patrolling a point
	bool bIsPatrollingPath;

	// The path anchor that is currently being moved towards
	uint8 CurrentPathAnchor;

	// Collision box used for vision
	class UBoxComponent* VisionBox;

	// Function to used when moving to any point
	void MoveToLocation(FVector Location);

	// Set vision box length and offset the location correctly
	void UpdateVision();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
