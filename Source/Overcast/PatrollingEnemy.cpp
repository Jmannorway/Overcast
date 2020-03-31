// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrollingEnemy.h"
#include "AIController.h"

// Sets default values
APatrollingEnemy::APatrollingEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrollingEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(GetController());

	AIController->MoveToLocation(FVector::ZeroVector, 50.f);
}

// Called every frame
void APatrollingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APatrollingEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

