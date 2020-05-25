// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerTouch.generated.h"

UCLASS()
class OVERCAST_API ATriggerTouch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerTouch();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTouch")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTouch")
		class UTriggerComponent* TriggerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerTouch")
		TSubclassOf<AActor> TriggeredBy;

	UFUNCTION(BlueprintNativeEvent)
		void Overlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "TriggerTouch")
		void TriggerAndDestroy();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
