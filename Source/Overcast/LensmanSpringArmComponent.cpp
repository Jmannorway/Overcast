// Fill out your copyright notice in the Description page of Project Settings.


#include "LensmanSpringArmComponent.h"
#include "CameraTrigger.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULensmanSpringArmComponent::ULensmanSpringArmComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Set inherited spring arm variables
	bInheritPitch = false;
	bInheritRoll = false;
	bInheritYaw = false;

	// Initialize hidden variables
	bChangingShot = false;
	CurrentShotTime = 0.f;
}

void ULensmanSpringArmComponent::SetCameraPosition(const FShot& NewShot)
{
	NextShot = NewShot;

	switch (NextShot.Instruction)
	{
	case EShotInstruction::None: Curve = &ULensmanSpringArmComponent::NoCurve; break;
	case EShotInstruction::Smooth: Curve = &ULensmanSpringArmComponent::SmoothCurve; break;
	case EShotInstruction::Exp: Curve = &ULensmanSpringArmComponent::ExponentialCurve; break;
	case EShotInstruction::InvExp: Curve = &ULensmanSpringArmComponent::InverseExponentialCurve; break;
	}

	CurrentShotTime = 0.f;

	bChangingShot = true;
}

void ULensmanSpringArmComponent::SetDefaultCameraPosition()
{
	SetCameraPosition(DefaultShot);
}

void ULensmanSpringArmComponent::BeginPlay()
{
	Super::BeginPlay();

	DefaultShot.Set(TargetArmLength, GetRelativeRotation(), GetRelativeLocation(), DefaultShot.Instruction, 1.f);
	PreviousShot = DefaultShot;
}

float ULensmanSpringArmComponent::NoCurve(float val)
{
	return val;
}

float ULensmanSpringArmComponent::SmoothCurve(float val)
{
	return 1.f - (FMath::Cos(val * PI) + 1.f) / 2.f;
}

float ULensmanSpringArmComponent::ExponentialCurve(float val)
{
	return FMath::Square(val);
}

float ULensmanSpringArmComponent::InverseExponentialCurve(float val)
{
	return 1.f - FMath::Square(1.f - val);
}

// Called every frame
void ULensmanSpringArmComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Perform linear interpolation between two camera positions using the specified curve function
	if (bChangingShot)
	{
		CurrentShotTime = FMath::Min(CurrentShotTime + DeltaTime, NextShot.Duration);

		float ShotCurveValue = (*this.*Curve)(CurrentShotTime / NextShot.Duration);

		TargetArmLength = FMath::Lerp(PreviousShot.Distance, NextShot.Distance, ShotCurveValue);
		TargetOffset = FMath::Lerp(PreviousShot.Offset, NextShot.Offset, ShotCurveValue);
		SetRelativeRotation(FMath::Lerp(PreviousShot.Rotation, NextShot.Rotation, ShotCurveValue));

		if (CurrentShotTime == NextShot.Duration)
		{
			PreviousShot = NextShot;
			bChangingShot = false;
		}
	}
}

/*
	FCameraPosition
*/

void FShot::Set(float ShotDistance, FRotator ShotRotation, FVector ShotOffset, EShotInstruction ShotInstruction, float ShotDuration)
{
	Distance = ShotDistance;
	Rotation = ShotRotation;
	Offset = ShotOffset;
	Instruction = ShotInstruction;
	Duration = ShotDuration;
}

FShot::FShot()
{
	Distance = 1000.f;
	Rotation = FRotator::ZeroRotator;
	Offset = FVector::ZeroVector;
	Instruction = EShotInstruction::Smooth;
	Duration = 1.f;
}

FShot::FShot(const FShot& Shot)
{
	*this = Shot;
}

FShot::FShot(float ShotDistance, FRotator ShotRotation, FVector ShotOffset, EShotInstruction ShotInstruction, float ShotDuration)
{
	Set(ShotDistance, ShotRotation, ShotOffset, ShotInstruction, ShotDuration);
}
