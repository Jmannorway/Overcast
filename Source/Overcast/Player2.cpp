// Fill out your copyright notice in the Description page of Project Settings.


#include "Player2.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayer2::APlayer2()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());


	SpringArm = CreateDefaultSubobject <USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 1000.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 2.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);




	Velocity = FVector(0.f);
	MaxSpeed = 600.f;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void APlayer2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Newlocation = GetActorLocation() + (Velocity * DeltaTime);
	SetActorLocation(Newlocation);

}

// Called to bind functionality to input
void APlayer2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis(TEXT("HorizontalMovement"), this, &APlayer2::HorizontalMovement);

	PlayerInputComponent->BindAxis(TEXT("VerticalMovement"), this, &APlayer2::VerticalMovement);

}

void APlayer2::HorizontalMovement(float input)
{
	Velocity.X = FMath::Clamp(input, -1.f, 1.f) * MaxSpeed;
}

void APlayer2::VerticalMovement(float input)
{
	Velocity.Y = FMath::Clamp(input, -1.f, 1.f) * MaxSpeed;
}

