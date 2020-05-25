// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Spell.h"
#include "ShadeSpell.generated.h"

UCLASS()
class OVERCAST_API AShadeSpell : public ASpell
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShadeSpell();

	virtual void Tick(float DeltaTime) override;

protected:

	// Sphere collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		class USphereComponent* SunCollision;

	// Sun mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		UStaticMeshComponent* SunMesh;
};