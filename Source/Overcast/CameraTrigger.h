// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LensmanSpringArmComponent.h"
#include "CameraTrigger.generated.h"

UENUM(BlueprintType)
enum class ECameraTriggerReaction : uint8
{
	Keep		UMETA(DisplayName = "Keep"),
	Custom		UMETA(DisplayName = "Custom"),
	Default		UMETA(DisplayName = "Default")
};

UCLASS()
class OVERCAST_API ACameraTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACameraTrigger();

	UFUNCTION(BlueprintCallable)
		ECameraTriggerReaction GetInsideReaction() const;

	UFUNCTION(BlueprintCallable)
		ECameraTriggerReaction GetOutsideReaction() const;

	const FShot& GetInsideShot() const;

	const FShot& GetOutsideShot() const;

protected:

	// The player will collide with this
	UPROPERTY(EditAnywhere, Category = "Trigger")
		class UBoxComponent* TriggerBox;

	// Shot to transition to when entering the box
	UPROPERTY(EditAnywhere, Category = "Trigger")
		ECameraTriggerReaction InsideReaction;

	// Shot to transition to when entering the box
	UPROPERTY(EditAnywhere, Category = "Trigger")
		FShot InsideShot;

	// Shot to transition to when leaving the box
	UPROPERTY(EditAnywhere, Category = "Trigger")
		ECameraTriggerReaction OutsideReaction;

	// Shot to transition to when leaving the box
	UPROPERTY(EditAnywhere, Category = "Trigger")
		FShot OutsideShot;

	// Editor-only sprite
	UPROPERTY(EditDefaultsOnly, AdvancedDisplay, Category = "Trigger")
		class UBillboardComponent* SpriteComponent;
};