// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlantPlatform.generated.h"

UENUM(BlueprintType)
enum class EGrowthPhase : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Growing		UMETA(DisplayName = "Growing"),
	Grown		UMETA(DisplayName = "Grown")
};

UCLASS()
class OVERCAST_API APlantPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlantPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// on construction override to visualize platform placements
	virtual void OnConstruction(const FTransform& Transform) override;

	// Scene component as root for easy offset
	class USceneComponent* DummyRoot;

	// Hitbox for the waterable plant
	UPROPERTY(EditAnywhere, Category = "Plant Platform")
		UStaticMeshComponent* PlantMesh;

	UFUNCTION()
		void OnPlantBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Plant Platform")
		class UBoxComponent* Platform;

	UPROPERTY(EditAnywhere, Category = "Plant Platform")
		float GrowthHeight;

	UPROPERTY(EditAnywhere, Category = "Plant Platform")
		float GrowthSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Plant Platform")
		EGrowthPhase GrowthPhase;

	float GrowthProgress;

	float OriginZ;

	void UpdatePlatformNumber();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
