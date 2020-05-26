// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantPlatformV2.generated.h"

enum class EGrowingPhase
{
	Idle,
	TopPlant,
	BottomPlant,
	Grown
};

UCLASS()
class OVERCAST_API APlantPlatformV2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantPlatformV2();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		float TopPlantSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		float BottomPlantSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		float GrowthHeight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	USceneComponent* DummyRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		UStaticMeshComponent* TopPlant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plant")
		UStaticMeshComponent* BottomPlant;

	EGrowingPhase GrowingPhase;
	float GrowingProgress;

	float TopPlantHeight;
	float BottomPlantHeight;

	UFUNCTION()
		void RainBeginOverlap(AActor* OverlappingActor, AActor* OtherActor);

	void AdvancePhase();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
