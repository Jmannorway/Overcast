// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectileEnemy.generated.h"

UCLASS()
class OVERCAST_API AProjectileEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AProjectileEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		class USkeletalMeshComponent* Mesh;

	// The area of the owl's vision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		class UBoxComponent* VisionBox;

	// Length of the charge before shooting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
		float ChargeLength;

	// Timer for charging
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
		float ChargeTimer;

	// 
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
		bool bInView;

	//
	UFUNCTION()
		void OnVisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//
	UFUNCTION()
		void OnVisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//
	UPROPERTY()
		class APlayer1* Player;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
