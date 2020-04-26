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

#define ____DEFAULT_MOVEMENT_SPEED 600.f
#define ____DEFAULT_SLIDE_SPEED 900.f

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
	if (UCharacterMovementComponent* PlayerMovement = GetCharacterMovement())
	{
		PlayerMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...
		PlayerMovement->RotationRate = FRotator(0.0f, 540.f, 0.0f); // ... at this rotation rate
		PlayerMovement->JumpZVelocity = 850.f;
		PlayerMovement->AirControl = 0.5f;
	}


	// Configure action sphere
	ActionSphere = CreateDefaultSubobject<USphereComponent>("ActionSphere");
	ActionSphere->SetupAttachment(RootComponent);
	ActionSphere->InitSphereRadius(72.f);


	// Pushable box default values
	bCanPushBox = false;
	bIsPushingBox = false;
	PushingStateSpeed = 200.f;


	// Bind overlap function
	ActionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayer1::OnActionSphereBeginOverlap);
	ActionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayer1::OnActionSphereEndOverlap);


	// Spell variables
	SpellSelector = CreateDefaultSubobject<USpellSelector>("SpellSelector");
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();

}

void APlayer1::ChangeSpell()
{
	
}

void APlayer1::OnActionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APushableBox* PushableBoxRef = Cast<APushableBox>(OtherActor))
	{
		bCanPushBox = true;
		PushableBox = PushableBoxRef;
	}
	else if (ACameraTrigger* CameraTriggerRef = Cast<ACameraTrigger>(OtherActor))
	{
		CameraTrigger = CameraTriggerRef;

		ECameraTriggerReaction Reaction = CameraTrigger->GetInsideReaction();

		if (Reaction == ECameraTriggerReaction::Default)
			CameraArm->SetDefaultCameraPosition();
		else if (Reaction == ECameraTriggerReaction::Custom)
			CameraArm->SetCameraPosition(CameraTrigger->GetInsideShot());
	}
}

void APlayer1::OnActionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PushableBox)
	{
		PushableBox = nullptr;
		bCanPushBox = false;
		bIsPushingBox = false;

		auto PlayerMovement = GetCharacterMovement();
		PlayerMovement->SetPlaneConstraintEnabled(false);
		PlayerMovement->MaxWalkSpeed = ____DEFAULT_MOVEMENT_SPEED;
	}
	else if (CameraTrigger)
	{
		ECameraTriggerReaction Reaction = CameraTrigger->GetOutsideReaction();

		if (Reaction == ECameraTriggerReaction::Default)
			CameraArm->SetDefaultCameraPosition();
		else if (Reaction == ECameraTriggerReaction::Custom)
			CameraArm->SetCameraPosition(CameraTrigger->GetOutsideShot());

		CameraTrigger = nullptr;
	}
}

void APlayer1::ActionPressed()
{
	if (PushableBox != nullptr)
	{
		bIsPushingBox = true;

		FVector BoxDirection = PushableBox->GetActorLocation() - GetActorLocation();
		BoxDirection.Normalize();

		UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();

		PlayerMovement->SetPlaneConstraintEnabled(true);

		if (FMath::Abs(BoxDirection.X) < 0.5f)
			PlayerMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);
		else
			PlayerMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

		PlayerMovement->MaxWalkSpeed = PushingStateSpeed;
	}
}

void APlayer1::ActionReleased()
{
	if (bIsPushingBox)
	{
		bIsPushingBox = false;
		
		auto PlayerMovement = GetCharacterMovement();
		PlayerMovement->SetPlaneConstraintEnabled(false);
		PlayerMovement->MaxWalkSpeed = ____DEFAULT_MOVEMENT_SPEED;
	}
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
	PlayerInputComponent->BindAction("ChangeSpell", IE_Pressed, this, &APlayer1::ChangeSpell);
	
	// Debug info button
	PlayerInputComponent->BindAction("Test", IE_Pressed, this, &APlayer1::ReportOnStuff);
	
	PlayerInputComponent->BindAxis("ForwardMovement", this, &APlayer1::ForwardMovement);
	PlayerInputComponent->BindAxis("HorizontalMovement", this, &APlayer1::HorizontalMovement);
}

void APlayer1::ForwardMovement(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		// Push box if possible
		if (bIsPushingBox)
			PushableBox->AddMovement(FVector::ForwardVector * Value * GetCharacterMovement()->MaxWalkSpeed);
	}
}
void APlayer1::HorizontalMovement(float Value)
{
	if ((Controller != nullptr) && (Value != 0.f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

	// Push box if possible
	if (bIsPushingBox)
		PushableBox->AddMovement(FVector::RightVector * Value * GetCharacterMovement()->MaxWalkSpeed);
}

// Spawn a raincloud with the desired offset
void APlayer1::Spell()
{
	FRotator Direction = GetActorRotation();

	// Cast rain cloud spell with offset
	switch (SpellSelector->GetSpellType())
	{
	case ESpellType::Rain:
		GetWorld()->SpawnActor<ARainCloud>(
			RainCloudSpell,
			GetActorLocation() + Direction.Vector() * SpellAheadOffset + SpellLocationOffset,
			Direction
			);
		break;

	case ESpellType::Wind: break;
	case ESpellType::Shade: break;
	}
	
}

void APlayer1::Slide()
{
	GetCharacterMovement()->MaxWalkSpeed = ____DEFAULT_SLIDE_SPEED;


}

void APlayer1::StopSlide()
{

	GetCharacterMovement()->MaxWalkSpeed = ____DEFAULT_MOVEMENT_SPEED;

}

USpellSelector* APlayer1::GetSpellSelector() const
{
	return SpellSelector;
}

/*
	USpellSelector code
*/

void USpellSelector::UnlockSpell(uint8 SpellIndex)
{
	if (SpellIndex < mNumber)
		mbKeychain[SpellIndex] = true;
}

void USpellSelector::UnlockSpell(ESpellType SpellType)
{
	if (uint8 SpellIndex = TypeToIndex(SpellType) < mNumber)
		mbKeychain[SpellIndex] = true;
}

USpellSelector::USpellSelector()
{
	mSpell.i = 0;

	for (bool& i : mbKeychain)
		i = false;

	mbKeychain[0] = true;
}

ESpellType USpellSelector::IndexToType(uint8 SpellIndex)
{
	return (SpellIndex < mNumber) ? static_cast<ESpellType>(SpellIndex) : ESpellType::INVALID;
}

uint8 USpellSelector::TypeToIndex(ESpellType SpellType)
{
	if (SpellType != ESpellType::NUMBER && SpellType != ESpellType::INVALID)
		return static_cast<uint8>(SpellType);
	else
		return static_cast<uint8>(ESpellType::INVALID);
}

ESpellType USpellSelector::GetSpellType() const
{
	return mSpell.t;
}

uint8 USpellSelector::GetSpellIndex() const
{
	return mSpell.i;
}

void USpellSelector::SetSpell(ESpellType SpellType)
{
	if (SpellType != ESpellType::NUMBER && SpellType != ESpellType::INVALID && mbKeychain[TypeToIndex(SpellType)])
		mSpell.t = SpellType;
}

void USpellSelector::SetSpell(uint8 SpellIndex)
{
	if (SpellIndex < mNumber && mbKeychain[SpellIndex])
		mSpell.i = SpellIndex;
}
