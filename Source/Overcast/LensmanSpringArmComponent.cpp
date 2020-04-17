// Fill out your copyright notice in the Description page of Project Settings.


#include "LensmanSpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULensmanSpringArmComponent::ULensmanSpringArmComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set inherited spring arm parameters
	bInheritPitch = false;
	bInheritRoll = false;
	bInheritYaw = false;

	// Initialize hidden variables
	bChangingShot = false;

	ShotLength = 0.f;
	ShotTime = 0.f;
}

void ULensmanSpringArmComponent::SetCameraPosition(const FShot& NewShot, EShotInstruction InstructionName, float Length)
{
	NextShot = NewShot;

	switch (InstructionName)
	{
	case EShotInstruction::None: Curve = &ULensmanSpringArmComponent::NoCurve; break;
	case EShotInstruction::Smooth: Curve = &ULensmanSpringArmComponent::SmoothCurve; break;
	case EShotInstruction::Exp: Curve = &ULensmanSpringArmComponent::ExponentialCurve; break;
	case EShotInstruction::InvExp: Curve = &ULensmanSpringArmComponent::InverseExponentialCurve; break;
	}

	ShotLength = Length;
	ShotTime = 0.f;

	bChangingShot = true;
}

void ULensmanSpringArmComponent::BeginPlay()
{
	DefaultShot.Set(TargetArmLength, GetRelativeRotation(), GetRelativeLocation());
	PreviousShot = DefaultShot;
}

float ULensmanSpringArmComponent::NoCurve(float val)
{
	return val;
}

float ULensmanSpringArmComponent::SmoothCurve(float val)
{
	return FMath::Sin(val * PI05);
}

float ULensmanSpringArmComponent::ExponentialCurve(float val)
{
	return 1.f;
}

float ULensmanSpringArmComponent::InverseExponentialCurve(float val)
{
	return 1.f;
}

// Called every frame
void ULensmanSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Perform linear interpolation between two camera positions using the specified curve funtion
	if (bChangingShot)
	{
		ShotTime = FMath::Min(ShotTime + DeltaTime, ShotLength);

		float ShotCurveValue = (*this.*Curve)(ShotTime / ShotLength);

		UE_LOG(LogTemp, Warning, TEXT("%f"), ShotCurveValue);

		if (ShotTime == ShotLength)
		{
			PreviousShot = NextShot;
			bChangingShot = false;
		}
	}
}

/*
	FCameraPosition
*/

void FShot::Set(float Distance, FRotator Rotation, FVector Location)
{
	ShotDistance = Distance;
	ShotRotation = Rotation;
	ShotLocation = Location;
}

FShot::FShot(const FShot& Shot)
{
	*this = Shot;
}

FShot::FShot(float Distance, FRotator Rotation, FVector Location)
{
	Set(Distance, Rotation, Location);
}

FShot::FShot(float&& Distance, FRotator&& Rotation, FVector&& Location)
{
	ShotDistance = Distance;
	ShotLocation = Location;
	ShotRotation = Rotation;
}
