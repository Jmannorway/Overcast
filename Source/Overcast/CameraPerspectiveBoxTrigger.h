// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraPerspectiveBoxTrigger.generated.h"

UCLASS()
class OVERCAST_API ACameraPerspectiveBoxTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraPerspectiveBoxTrigger();

	UPROPERTY(EditAnywhere, Category = "Trigger")
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = "Trigger")
		FRotator RotationOffset;

	UPROPERTY(EditAnywhere, Category = "Trigger")
		float RadiusOffset;

	UPROPERTY(EditAnywhere, Category = "Trigger")
		FVector LocationOffset;

	UPROPERTY(EditAnywhere, Category = "Trigger")
		bool bOnBeginOverlap;

	UPROPERTY(EditAnywhere, Category = "Trigger")
		bool bOnEndOverlap;

	// Editor-only sprite
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category = "Trigger")
		class UBillboardComponent* SpriteComponent;

	// Functions that call functions inside the player to alter the camera
	UFUNCTION()
		void OnPlayerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnPlayerEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
