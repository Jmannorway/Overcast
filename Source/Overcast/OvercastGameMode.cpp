// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Player1.h"
#include "GameFramework/PlayerController.h"
#include "Checkpoint.h"

AOvercastGameMode::AOvercastGameMode()
{
	CurrentLevelIndex = 0;
	CurrentCheckpointIndex = -1;
}

void AOvercastGameMode::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("This is actually fucking ticking"));
}

void AOvercastGameMode::Respawn()
{
	// Destroy player
	if (auto PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		PlayerPawn->Destroy();

	APawn* Player = nullptr;

	if (CurrentCheckpointIndex > -1 && CheckpointClass)
	{
		Player = SpawnAtCheckpoint();
	}

	if (!Player)
	{
		Player = SpawnAtPlayerStart();
		UE_LOG(LogTemp, Warning, TEXT("Couldn't spawn player at checkpoint"));

		if (!Player)
		{
			Player = SpawnAtZero();
			UE_LOG(LogTemp, Warning, TEXT("Couldn't spawn player at player start"));
		}
	}

	// Possess if spawned
	if (Player)
	{
		UGameplayStatics::GetPlayerController(this, 0)->Possess(Player);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn player"));
	}
}

APawn* AOvercastGameMode::SpawnAtCheckpoint()
{
	TArray<AActor*> Checkpoint;
	UGameplayStatics::GetAllActorsOfClass(this, CheckpointClass, Checkpoint);

	bool bCorrectCheckpoint = false;
	int32 CheckpointIndex = Checkpoint.Num() - 1;
	ACheckpoint* CastCheckpoint;

	UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn at checkpoint"));

	for (int32 i = Checkpoint.Num() - 1; i >= 0; i--)
	{
		CastCheckpoint = CastChecked<ACheckpoint>(Checkpoint[i]);

		if (CastCheckpoint->GetCheckpointIndex() == CurrentCheckpointIndex)
		{
			return GetWorld()->SpawnActor<APlayer1>(DefaultPawnClass, CastCheckpoint->GetSpawnLocation(), CastCheckpoint->GetSpawnRotation());
		}
	}

	return nullptr;
}

APawn* AOvercastGameMode::SpawnAtPlayerStart()
{
	// Spawn player at a player start or zero position
	if (auto PlayerStart = FindPlayerStart(UGameplayStatics::GetPlayerController(this, 0)))
		return GetWorld()->SpawnActor<APlayer1>(DefaultPawnClass, PlayerStart->GetActorLocation(), FRotator::ZeroRotator);
	else
		return nullptr;
}

APawn* AOvercastGameMode::SpawnAtZero()
{
	return GetWorld()->SpawnActor<APlayer1>(DefaultPawnClass, FVector::ZeroVector, FRotator::ZeroRotator);;
}
