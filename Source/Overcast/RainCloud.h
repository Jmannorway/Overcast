// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RainCloud.generated.h"

/*
	made by ya boi. don't steal. i'm proud.
*/

UCLASS()
class OVERCAST_API ARainCloud : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARainCloud();

protected:
	// Particle system used for the cloud itself
	UPROPERTY(EditAnywhere, Category = "Cloud")
		UParticleSystemComponent* CloudParticles;

	// Movement speed
	UPROPERTY(EditAnywhere, Category = "Cloud")
		float CloudSpeed;

	// For how long the cloud will move before disappearing
	UPROPERTY(EditAnywhere, Category = "Cloud")
		int32 CloudLife;

	// Particle system used for the rain
	UPROPERTY(EditAnywhere, Category = "Rain")
		UParticleSystemComponent* RainParticles;

	// How long the floor is going to stay wet before drying
	UPROPERTY(EditAnywhere, Category = "Rain")
		int32 RainLife;

	// How far the rain can reach down
	UPROPERTY(EditAnywhere, Category = "Rain")
		float RainReachDepth;

	// The width of the rain area
	UPROPERTY(EditAnywhere, Category = "Rain")
		float RainReachWidth;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	class UBoxComponent* RainArea;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
