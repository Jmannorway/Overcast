// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TriggerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCAST_API UTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerComponent();

	UFUNCTION(BlueprintCallable, Category = "Trigger")
		void Trigger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		int32 TriggerIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		TSubclassOf<class AStaticMeshTrigger> TriggerableClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
		bool bTriggerOnDestroy;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
