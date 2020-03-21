// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyPatrol.generated.h"

UENUM(BlueprintType)
enum class EEP_MovementState : uint8
{
	PatrolPath		UMETA(DisplayName = "Patrol path"),
	MoveToTarget	UMETA(DisplayName = "Move to target"),
	Attack			UMETA(DisplayName = "Attack target"),
	MoveToPath		UMETA(DisplayName = "Move to path"),
	NUMBER
};

UCLASS()
class OVERCAST_API AEnemyPatrol : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyPatrol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// Path and anchors ///

	// Path anchor points
	UPROPERTY(EditAnywhere, Category = "Path") TArray<FVector> Anchor;

	// Start anchor
	UPROPERTY(EditAnywhere, Category = "Path") int32 StartAnchor;

	// Where to start patrolling between this and the next path (0 - 1)
	UPROPERTY(EditAnywhere, Category = "Path") float StartAnchorOffset;

	/// AI ///

	UPROPERTY(VisibleAnywhere, Category = "AI") class AAIController *EnemyController;

	UPROPERTY(VisibleAnywhere, Category = "AI") EEP_MovementState State;

	UFUNCTION(BlueprintCallable) void SetMovementState(EEP_MovementState NewState);

private:
	UPROPERTY() int32 NextAnchor;

	UFUNCTION() int32 GetNextAnchor(int32 AnchorIndex) const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
