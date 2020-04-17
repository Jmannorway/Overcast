// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
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
#include "UObject/Object.h"
#include "Player2.h"
#include "Engine/EngineTypes.h"
#include "LensmanSpringArmComponent.h"
#include "CameraTrigger.h"

// Sets default values
APlayer1::APlayer1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	
	// Set spring arm
	CameraArm = CreateDefaultSubobject<ULensmanSpringArmComponent>("SpringArm");
	CameraArm->TargetArmLength = 1000.f; //camera follows player at this distance
	CameraArm->bEnableCameraLag = true;
	CameraArm->CameraLagSpeed = 2.0f;
	CameraArm->SetupAttachment(RootComponent);

	// Set camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm);

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


	// Configure action sphere
	ActionSphere = CreateDefaultSubobject<USphereComponent>("ActionSphere");
	ActionSphere->SetupAttachment(RootComponent);
	ActionSphere->InitSphereRadius(72.f);


	// Pushable box default values
	bCanPushBox = false;
	bIsPushingBox = false;


	// Unfreeze player
	MovementConstraintVector = FVector(1.f, 1.f, 1.f);
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APlayer1::OnBeginOverlap);
	OnActorEndOverlap.AddDynamic(this, &APlayer1::OnEndOverlap);

	ActionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayer1::OnActionSphereBeginOverlap);
	ActionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayer1::OnActionSphereEndOverlap);
}

void APlayer1::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ACameraTrigger* CameraTrigger = Cast<ACameraTrigger>(OtherActor))
	{
		CameraArm->SetCameraPosition(CameraTrigger->NewShot, CameraTrigger->ShotInstruction, CameraTrigger->TransitionLength);
	}
}

void APlayer1::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
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
	if (PushableBox != nullptr)
	{
		bIsPushingBox = true;

		FVector BoxDirection = PushableBox->GetActorLocation() - GetActorLocation();
		BoxDirection.Normalize();
		BoxDirection.X = FMath::Abs(BoxDirection.X);
		BoxDirection.Y = FMath::Abs(BoxDirection.Y);

		if (BoxDirection.X > 0.5f)
			SetAxisConstraint(EMovementConstraintAxis::Y);
		else
			SetAxisConstraint(EMovementConstraintAxis::X);

		GetCharacterMovement()->MaxWalkSpeed = 450.f;
	}
}

void APlayer1::ActionReleased()
{
	if (bIsPushingBox)
	{
		bIsPushingBox = false;
		RemoveAxisConstraint();
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void APlayer1::SetAxisConstraint(EMovementConstraintAxis Axis)
{
	switch (Axis)
	{
	case EMovementConstraintAxis::X: MovementConstraintVector = FVector(0.f, 1.f, 1.f);  break;
	case EMovementConstraintAxis::Y: MovementConstraintVector = FVector(1.f, 0.f, 1.f);  break;
	case EMovementConstraintAxis::Z: MovementConstraintVector = FVector(1.f, 1.f, 0.f);  break;
	}
}

FVector APlayer1::GetAxisConstraint() const
{
	return MovementConstraintVector;
}

void APlayer1::RemoveAxisConstraint()
{
	MovementConstraintVector = FVector(1.f, 1.f, 1.f);
}

void APlayer1::ReportOnStuff()
{
	UE_LOG(LogTemp, Warning, TEXT("CanMoveBox: %d\nMovingBox: %d\nBoxPointer: %X"), bCanPushBox, bIsPushingBox, PushableBox);
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

	if (bIsPushingBox && PushableBox)
	{
		PushableBox->AddActorLocalOffset(GetActorLocation() - PreviousLocation);
	}

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
	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &APlayer1::ActionPressed);
	PlayerInputComponent->BindAction("Action", IE_Released, this, &APlayer1::ActionReleased);
	
	// Debug info button
	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &APlayer1::ReportOnStuff);
	
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
