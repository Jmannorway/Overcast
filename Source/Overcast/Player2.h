// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Player2.generated.h"

UCLASS()
class OVERCAST_API APlayer2 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayer2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, Category = "Pawn Movement")
		float MaxSpeed;

	

private:

	void HorizontalMovement(float input);

	void VerticalMovement(float input);

	FVector Velocity;

};
