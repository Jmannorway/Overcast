// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PushableBox.generated.h"

/**
 * 
 */
UCLASS()
class OVERCAST_API APushableBox : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	APushableBox();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
		bool bIsFalling;

	UFUNCTION()
		void AddMovement(const FVector& Movement);

protected:

	UPROPERTY(EditAnywhere)
		float Gravity;

private:
	FVector NextMovement;
};
