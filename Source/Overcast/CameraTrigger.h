// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LensmanSpringArmComponent.h"
#include "CameraTrigger.generated.h"

UCLASS()
class OVERCAST_API ACameraTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraTrigger();

	// The player will collide with this
	UPROPERTY(EditAnywhere, Category = "Trigger")
		class UBoxComponent* TriggerBox;

	// New shot for the camera to transition to
	UPROPERTY(EditAnywhere, Category = "Trigger")
		FShot NewShot;

	// How to transition to the new shot
	UPROPERTY(EditAnywhere, Category = "Trigger")
		EShotInstruction ShotInstruction;

	// Length of the transition to the new view
	UPROPERTY(EditAnywhere, Category = "Trigger")
		float TransitionLength;

	// If true, camera returns to default state after player exits the trigger
	UPROPERTY(EditAnywhere, Category = "Trigger")
		bool bReturnToDefault;

	// Editor-only sprite
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category = "Trigger")
		class UBillboardComponent* SpriteComponent;
};