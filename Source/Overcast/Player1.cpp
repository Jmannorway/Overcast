// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RainCloud.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/MovementComponent.h"
#include "PushableBox.h"
#include "GameplayTask.h"
#include "Player2.h"

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
	
<<<<<<< HEAD

	// Configure action sphere
	ActionSphere = CreateDefaultSubobject<USphereComponent>("ActionSphere");
	ActionSphere->SetupAttachment(RootComponent);
	ActionSphere->InitSphereRadius(72.f);


	// Pushable box default values
	bCanPushBox = false;
	bIsPushingBox = false;


	// Unfreeze player
	RemoveMovementConstraints();
}

void APlayer1::OnActionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APushableBox>(OtherActor))
	{
		// Get pointer to the pushable box in question
		PushableBox = CastChecked <APushableBox>(OtherActor);
		bCanPushBox = true;
	}
}

void APlayer1::OnActionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PushableBox)
	{
		PushableBox = nullptr;
		bCanPushBox = false;
	}
}

void APlayer1::ActionPressed()
{
	if (PushableBox)
	{
		bIsPushingBox = true;
		AddMovementConstraints(true, false, false);
	}
}

void APlayer1::ActionReleased()
{
	if (PushableBox)
	{
		RemoveMovementConstraints();
		bIsPushingBox = false;
	}
}

void APlayer1::AddMovementConstraints()
{
	MovementConstraintVector = FVector(0.f, 0.f, 0.f);
}

void APlayer1::AddMovementConstraints(bool X, bool Y, bool Z)
{
	if (X) MovementConstraintVector.X = 0.f;
	if (Y) MovementConstraintVector.Y = 0.f;
	if (Z) MovementConstraintVector.Z = 0.f;
}

void APlayer1::RemoveMovementConstraints()
{
	MovementConstraintVector = FVector(1.f, 1.f, 1.f);
}

void APlayer1::RemoveMovementConstraints(bool X, bool Y, bool Z)
{
	if (X) MovementConstraintVector.X = 1.f;
	if (Y) MovementConstraintVector.Y = 1.f;
	if (Z) MovementConstraintVector.Z = 1.f;
}

FVector APlayer1::GetMovementConstaints() const
{
	return MovementConstraintVector;
=======
	
>>>>>>> 15b601cc2aa5729a88329064553516caa9ae3a44
}



// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();
	
	ActionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayer1::OnActionSphereBeginOverlap);
	ActionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayer1::OnActionSphereEndOverlap);
}

// Called every frame
void APlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//IsPendingKill();
 

	//if (IsPendingKill())
	//{
		
	//	Restart(PlayerDestroyed);
	//	PlayerDestroyed = false;
	//}

<<<<<<< HEAD
	if (bIsPushingBox && PushableBox)
	{
		PushableBox->AddActorLocalOffset(PreviousLocation - GetActorLocation());
	}
=======
>>>>>>> 15b601cc2aa5729a88329064553516caa9ae3a44

	PreviousLocation = GetActorLocation();
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
		AddMovementInput(Direction * MovementConstraintVector, Value);
	}
}
void APlayer1::HorizontalMovement(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction * MovementConstraintVector, Value);
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
