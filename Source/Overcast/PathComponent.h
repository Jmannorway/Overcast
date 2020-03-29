// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathComponent.generated.h"

class ATargetPoint;
class UArrowComponent;

UENUM()
enum class EPathVisualizerDirection : uint8 {Up, Right, Forward, Back, Down, Left};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OVERCAST_API UPathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Path anchors
	UPROPERTY(EditAnywhere, Category = "Path")
		TArray<ATargetPoint*> Anchors;

	// Visualizer Arrow
	UPROPERTY(VisibleAnywhere, Category = "Visualization")
		UArrowComponent* VisualizerArrow;

public:	
	// Get the position of a path anchor
	UFUNCTION()
		FVector GetPathAnchor(uint32 Index) const;

	// Get the total number of path anchors
	UFUNCTION()
		uint32 GetPathAnchorNumber() const;

	// Visualize the path
	virtual void VisualizePath();
	virtual void VisualizePath(const FColor& Color);
	virtual void VisualizePath(const FColor& Color, uint32 StartAnchor, float StartAnchorOffset);
	virtual void VisualizePath(float Offset, EPathVisualizerDirection OffsetDirection, const FColor& Color, uint32 StartAnchor, float StartAnchorOffset, float ArrowOffsetMultiplier = 0.5f);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
