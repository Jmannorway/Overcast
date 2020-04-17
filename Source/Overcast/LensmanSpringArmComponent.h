// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "LensmanSpringArmComponent.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EShotInstruction : uint8
{
	None		UMETA(DisplayName = "Linear Interpolation"),
	Smooth		UMETA(DisplayName = "Smooth Linear Interpolation"),
	Exp			UMETA(DisplayName = "Exponential Linear Interpolation"),
	InvExp		UMETA(DisplayName = "Inverse Exponential Linear Interpolation")
};

USTRUCT(BlueprintType)
struct FShot
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere) float ShotDistance;
	UPROPERTY(EditAnywhere) FRotator ShotRotation;
	UPROPERTY(EditAnywhere) FVector ShotLocation;

	void Set(float Distance, FRotator Rotation, FVector Location);

	FShot() { ; }
	FShot(const FShot& Shot);
	FShot(float Distance, FRotator Rotation, FVector Location);
	FShot(float&& Distance, FRotator&& Rotation, FVector&& Location);
};

UCLASS()
class OVERCAST_API ULensmanSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:

	ULensmanSpringArmComponent();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Cinematography")
		FShot DefaultShot;

	UPROPERTY(EditAnywhere, Category = "Cinematography")
		FShot NextShot;

	UPROPERTY(EditAnywhere, Category = "Cinematography")
		FShot PreviousShot;

private:

	/*
		This is some demonic c++ black magic shit.
		tbh, I will keep away from the uncanny valley in the future
	*/

	bool bChangingShot;

	static constexpr float PI05 = 1.5707963f;
	static constexpr float PI025 = 0.7853981f;

	// The variable to fold the instruction function
	float(ULensmanSpringArmComponent::* Curve)(float);

	// Instruction functions
	float NoCurve(float val);
	float SmoothCurve(float val);
	float ExponentialCurve(float val);
	float InverseExponentialCurve(float val);

	// Instruction time variables
	float ShotTime;
	float ShotLength;

public:

	// Asks the LensmanSpringArmComponent to change camera position
	UFUNCTION(BlueprintCallable, Category = "Cinematography")
		void SetCameraPosition(const FShot& NewShot, EShotInstruction InstructionName, float Length);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
