// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RainCloud.h"

// Sets default values
APlayer1::APlayer1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//create Camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 1000.f; //camera follows player at this distance
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 2.0f;
	CameraBoom->bUsePawnControlRotation = true; //Rotate arm based on controller

	//Create follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//Atach the camera to the end of the boom and let the boom adjust to match
	// the controller orientation
	FollowCamera->bUsePawnControlRotation = false;


	//Don't rotate when controller rotate
	//let that just affect camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;


	//Configure character movement 
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f); // ... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 850.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->MaxWalkSpeed = 900.f;
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayer1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("RainSpell", IE_Pressed, this, &APlayer1::Spell);
	PlayerInputComponent->BindAction("Slide", IE_Pressed, this, &APlayer1::Slide);
	PlayerInputComponent->BindAction("Slide", IE_Released, this, &APlayer1::StopSlide);
	


	PlayerInputComponent->BindAxis("VerticalMovement", this, &APlayer1::VerticalMovement);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &APlayer1::HorizontalMovement);
}

void APlayer1::VerticalMovement(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void APlayer1::HorizontalMovement(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

// Spawn a raincloud with the desired offset
void APlayer1::Spell()
{
	FRotator Direction = GetActorRotation();

	// Cast rain cloud spell with offset
	GetWorld()->SpawnActor<ARainCloud>(
		RainCloudSpell,
		GetActorLocation() + Direction.Vector() * SpellAheadOffset + SpellLocationOffset,
		Direction
	);
}

void APlayer1::Slide()
{
	GetCharacterMovement()->MaxWalkSpeed = 900.f;


}

void APlayer1::StopSlide()
{

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

}
