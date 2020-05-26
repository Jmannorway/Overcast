// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "WindSpell.generated.h"

UCLASS()
class OVERCAST_API AWindSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWindSpell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Wind Spell")
		class UStaticMeshComponent* WindMesh;

	UPROPERTY(EditAnywhere, Category = "Wind Spell")
		class UBoxComponent* WindArea;

	UPROPERTY(EditAnywhere, Category = "Wind Spell")
		float WindSpeed;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
