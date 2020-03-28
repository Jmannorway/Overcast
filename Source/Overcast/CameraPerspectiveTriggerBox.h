// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "CameraPerspectiveTriggerBox.generated.h"

/**
 * coded by ya boi
 */
UCLASS()
class OVERCAST_API ACameraPerspectiveTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
    // How far out/in to move the camera
    UPROPERTY(EditAnywhere, Category = "Perspective parameters")
        float RadiusChange;

    //
    UPROPERTY(EditAnywhere, Category = "Perspective parameters")
        FRotator RotationOffset;

    //
    UPROPERTY(EditAnywhere, Category = "Perspective parameters")
        float VerticalAngleChange;
};
