// Fill out your copyright notice in the Description page of Project Settings.


#include "OvercastGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Player1.h"
#include "GameFramework/PlayerController.h"

void AOvercastGameMode::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("This is actually fucking ticking"));
}

void AOvercastGameMode::Respawn()
{
	// Destroy player
	if (auto PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
		PlayerPawn->Destroy();

	// Get current player controller & player start
	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	auto PlayerStart = FindPlayerStart(PlayerController);

	// Spawn & possess player
	APlayer1* Player;

	if (PlayerStart)
		Player = GetWorld()->SpawnActor<APlayer1>(DefaultPawnClass, PlayerStart->GetActorLocation(), FRotator::ZeroRotator);
	else
		Player = GetWorld()->SpawnActor<APlayer1>(DefaultPawnClass, FVector::ZeroVector, FRotator::ZeroRotator);

	PlayerController->Possess(Player);
}