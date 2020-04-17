// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LevelStreamerTrigger.generated.h"

/**
 *
 */
UCLASS()
class OVERCAST_API ALevelStreamerTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	ALevelStreamerTrigger();

protected:
	// Begin play to bind overlap delegate
	virtual void BeginPlay() override;

	UFUNCTION()
		void TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Level Streaming")
		FName LevelToLoad;

	UPROPERTY(EditAnywhere, Category = "Level Streaming")
		FName LevelToUnload;
};
