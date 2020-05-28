// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Pushableblock.generated.h"

UCLASS()
class OVERCAST_API APushableblock : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APushableblock();

protected:

	UPROPERTY(EditAnywhere, Category = "Push Block")
		class UBoxComponent* BlockBox;


public:	


};
