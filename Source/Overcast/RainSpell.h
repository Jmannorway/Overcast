// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "RainSpell.generated.h"

/**
 * 
 */
UCLASS()
class OVERCAST_API ARainSpell : public ASpell
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ARainSpell();

protected:

	UPROPERTY(EditAnywhere, Category = "Rain Spell")
		float CloudSpeed;

	UPROPERTY(EditAnywhere, Category = "Rain Spell")
		class UBoxComponent* RainArea;

	UPROPERTY(EditAnywhere, Category = "Rain Spell")
		class UParticleSystemComponent* CloudParticles;

	UPROPERTY(EditAnywhere, Category = "Rain Spell")
		UStaticMeshComponent* CloudMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
