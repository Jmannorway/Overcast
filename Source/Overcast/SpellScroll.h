// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellSelector.h"
#include "SpellScroll.generated.h"

UCLASS()
class OVERCAST_API ASpellScroll : public AActor
{
	GENERATED_BODY()
	

public:

	ASpellScroll();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		class USphereComponent* ScrollCollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		class UTriggerComponent* TriggerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		UStaticMeshComponent* ScrollMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		ESpellType SpellToUnlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		float ScrollRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		bool bDestroyOnCollect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scroll")
		bool bCollectable;

	UFUNCTION()
		void ScrollSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	

	UFUNCTION(BlueprintCallable, Category = "Scroll")
		void SetCollectable(bool bNewCollectability);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
