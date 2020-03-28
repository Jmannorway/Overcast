// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPatrol.generated.h"

UCLASS()
class OVERCAST_API AEnemyPatrol : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyPatrol();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Overriding this in order visually represent path lines in the editor
	virtual void OnConstruction(const FTransform& Transform) override;

	/// Path & anchor variables

	// Path anchor points
	UPROPERTY(EditAnywhere, Category = "Path")
		TArray<FVector> Anchors;

	//
	UPROPERTY(EditAnywhere, Category = "Path")
		FColor PathColorInEditor;

private:
	UPROPERTY()
		class UArrowComponent* PathHelperArrow;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
