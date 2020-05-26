// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingLightningProjectile.generated.h"

UCLASS()
class OVERCAST_API AHomingLightningProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHomingLightningProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Lightning particles
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning Projectile")
		class UParticleSystemComponent* ParticleSystem;

	// The actor being targeted
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning Projectile")
		AActor* TargetActor;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning Projectile")
		USceneComponent* ProjectileStart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lightning Projectile")
		USceneComponent* ProjectileEnd;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
