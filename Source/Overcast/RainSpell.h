// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RainSpell.generated.h"

UCLASS()
class OVERCAST_API ARainSpell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARainSpell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void Overlap(UPrimitiveComponent* Overlapcomp, AActor* OtherActor, UPrimitiveComponent* Othercomp, int32 OtherBodyIndex, bool bFSweep, const FHitResult& SweepR);

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* SpellMesh;

	UPROPERTY(VisibleAnywhere)
		class UBoxComponent* SpellCollider;

};
