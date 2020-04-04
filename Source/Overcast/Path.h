// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Path.generated.h"

class ATargetPoint;
class UArrowComponent;

UENUM(BlueprintType)
enum class EPathOffsetDirection : uint8 
{ 
	Right		UMETA(DisplayName = "Right"),
	Left		UMETA(DisplayName = "Left"),
	Forward		UMETA(DisplayName = "Forward"),
	Backward	UMETA(DisplayName = "Backward"),
	Up			UMETA(DisplayName = "Up"),
	Down		UMETA(DisplayName = "Down")
};

USTRUCT(BlueprintType)
struct FAnchorPoint
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
		uint8 AnchorIndex;

	UPROPERTY(EditAnywhere)
		float AnchorOffset;
};

UCLASS()
class OVERCAST_API APath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APath();

protected:
	// Override on construction script for path visualization
	virtual void OnConstruction(const FTransform& Transform);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Does not have functionality yet
	UPROPERTY(EditAnywhere, Category = "Path")
		bool bSelfIsFirstAnchor;

	// Path anchors
	UPROPERTY(EditAnywhere, Category = "Path")
		TArray<ATargetPoint*> Anchor;

	// Points defined on the path for various uses
	UPROPERTY(EditAnywhere, Category = "Path")
		TArray<FAnchorPoint> Point;

	// Path visualization color
	UPROPERTY(EditAnywhere, Category = "Path Visuals")
		FColor PathColor;

	UPROPERTY(EditAnywhere, Category = "Path Visuals")
		float PathOffset;

	UPROPERTY(EditAnywhere, Category = "Path Visuals")
		EPathOffsetDirection PathOffsetDirection;

	UArrowComponent* DirectionalArrow;

public:

	UFUNCTION(Category = "Path", BlueprintCallable)
		uint8 GetAnchorNumber() const;

	UFUNCTION(Category = "Path", BlueprintCallable)
		uint8 GetNextAnchorIndex(uint8 Index) const;

	UFUNCTION(Category = "Path", BlueprintCallable)
		uint8 GetPreviousAnchorIndex(uint8 Index) const;

	UFUNCTION(Category = "Path", BlueprintCallable)
		FVector GetAnchorLocation(uint8 Index) const;
		FVector GetAnchorLocation(uint8 Index, float Offset) const;

	UFUNCTION(Category = "Path", BlueprintCallable)
		void GetAllAnchorLocations(TArray<FVector>& OutArray) const;

	UFUNCTION(Category = "Path", BlueprintCallable)
		uint8 GetPointNumber() const;

	UFUNCTION(Category = "Path", BlueprintCallable)
		uint8 GetPointAnchor(uint8 Index) const;

	UFUNCTION(Category = "Path", BlueprintCallable)
		FVector GetPointLocation(uint8 Index) const;

	virtual void VisualizePath(const FColor& Color = FColor::Magenta, float Offset = 96.f, EPathOffsetDirection OffsetDirection = EPathOffsetDirection::Up, float ArrowOffsetMultiplier = 0.5f) const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
