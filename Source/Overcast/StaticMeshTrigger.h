// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticMeshTrigger.generated.h"

UCLASS()
class OVERCAST_API AStaticMeshTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStaticMeshTrigger();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Trigger")
		void Trigger();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
		int32 GetTriggerIndex() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		class UTriggerMovementComponent* TriggerMovement;

	// How far in any direction to move when triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		FVector TriggerMovement;

	// How to rotate when triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		FRotator TriggerRotation;

	// Translation duration in frames
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		int32 TriggerTime;

	// The index of the trigger component to get triggeredby
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		int32 TriggerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		bool bDestroyAfterMoving;

private:

	UPROPERTY()
		bool bTriggered;

	UPROPERTY()
		FVector TriggerMovementStep;

	UPROPERTY()
		FRotator TriggerRotationStep;

	// Mesh component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		UStaticMeshComponent* Mesh;
z
};
