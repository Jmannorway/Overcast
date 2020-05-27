// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightningProjectile.generated.h"

UCLASS()
class OVERCAST_API ALightningProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALightningProjectile();

	// Target actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		AActor* TargetActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USphereComponent* KillSphere;

	UFUNCTION()
		void OnKillSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
		float ProjectileSpeed;

private:

	UPROPERTY()
		float CurrentProjectileRange;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
