// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RainCloud.generated.h"

UCLASS()
class OVERCAST_API ARainCloud : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARainCloud();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Rain Cloud")
		int32 CloudLife;

	UPROPERTY(EditAnywhere, Category = "Rain Cloud")
		float CloudSpeed;

	UPROPERTY(EditAnywhere, Category = "Rain Cloud")
		class UBoxComponent* WetArea;

	UPROPERTY(EditAnywhere, Category = "Rain Cloud")
		class UParticleSystemComponent* CloudParticles;

private:

	// Counter keeping track of how long the cloud has been alive
	int32 CloudLifeCount;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
