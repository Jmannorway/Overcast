// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class OVERCAST_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
		USceneComponent* OffsetComponent;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
		class UBoxComponent* Box;

	UPROPERTY()
		class UBillboardComponent* Sprite;

	UPROPERTY()
		class UArrowComponent* Arrow;

	UPROPERTY()
		bool bIsTalking;


	UPROPERTY(EditAnywhere, Category = "Checkpoint")
		int32 CheckpointIndex;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
		FVector SpawnLocationOffset;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
		FRotator SpawnRotation;

public:

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
		int32 GetCheckpointIndex() const;

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
		FVector GetSpawnLocation() const;

	UFUNCTION(BlueprintCallable, Category = "Checkpoint")
		FRotator GetSpawnRotation() const;

	UFUNCTION()
		void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
