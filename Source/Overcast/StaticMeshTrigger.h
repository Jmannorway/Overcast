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

	UFUNCTION(BlueprintCallable, Category = "Trigger")
		void Trigger();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
		int32 GetTriggerIndex() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// How far in any direction to move when triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		FVector TriggerMovement;

	// The speed at which to move after being triggered
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		float TriggerMovementSpeed;

	// The index of the trigger component to get triggeredby
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		int32 TriggerIndex;

	// Mesh component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		UStaticMeshComponent* Mesh;

private:

	UPROPERTY()
		bool bTriggered;

	UPROPERTY()
		int32 TriggerMovementTime;

	UPROPERTY()
		FVector TriggerMovementStep;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
