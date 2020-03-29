// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPatrol.generated.h"

class UPathComponent;

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

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
		UStaticMeshComponent* CharacterMesh;

	// Path component
	UPROPERTY(EditAnywhere, Category = "Path")
		UPathComponent* Path;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
