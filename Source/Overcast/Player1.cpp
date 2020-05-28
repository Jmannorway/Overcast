// Fill out your copyright notice in the Description page of Project Settings.


#include "Player1.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
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
#include "GameHUD.h"
#include "GameFramework/GameModeBase.h"
#include "OvercastMainGameMode.h"
#include "SpellSelector.h"
#include "SpellScroll.h"
#include "Components/CapsuleComponent.h"
#include "Spell.h"

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


	// Set up collision for the capsule component
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayer1::CapsuleComponentBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayer1::CapsuleComponentEndOverlap);


	// Set camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraArm);


	//Don't rotate when controller rotate
	//let that just affect camera
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	
	//Configure character movement 
	UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();
	PlayerMovement->bOrientRotationToMovement = true; // Character moves in the direction of input...
	PlayerMovement->RotationRate = FRotator(0.0f, 540.f, 0.0f); // ... at this rotation rate
	PlayerMovement->JumpZVelocity = 1450.f;
	PlayerMovement->AirControl = 0.5f;
	PlayerMovement->GravityScale = 2.5f;


	// Spell selector
	SpellSelector = CreateDefaultSubobject<USpellSelector>("SpellSelector");
	SpellSelector->UnlockSpell(ESpellType::Rain);
	SpellSelector->UnlockSpell(ESpellType::Wind);
	SpellSelector->UnlockSpell(ESpellType::Shade);


	// Dash variables
	DashSpeed = 2000.f;
	DashLength = 1.f;
	DashCooldown = 5.f;
}

// Called when the game starts or when spawned
void APlayer1::BeginPlay()
{
	Super::BeginPlay();

	if (auto HUD = CastChecked<AGameHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD()))
	{
		CurrentHUD = HUD;
	}
}

void APlayer1::CapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACameraTrigger* CameraTriggerRef = Cast<ACameraTrigger>(OtherActor))
	{
		CameraTrigger = CameraTriggerRef;

		ECameraTriggerReaction Reaction = CameraTrigger->GetInsideReaction();

		if (Reaction == ECameraTriggerReaction::Default)
			CameraArm->SetDefaultCameraPosition();
		else if (Reaction == ECameraTriggerReaction::Custom)
			CameraArm->SetCameraPosition(CameraTrigger->GetInsideShot());
	}
}

void APlayer1::CapsuleComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CameraTrigger == OtherActor)
	{
		ECameraTriggerReaction Reaction = CameraTrigger->GetOutsideReaction();

		if (Reaction == ECameraTriggerReaction::Default)
			CameraArm->SetDefaultCameraPosition();
		else if (Reaction == ECameraTriggerReaction::Custom)
			CameraArm->SetCameraPosition(CameraTrigger->GetOutsideShot());

		CameraTrigger = nullptr;
	}
}

void APlayer1::SetPlayerMovementState(EPlayerMovementState NewState)
{
	switch (NewState)
	{
	case EPlayerMovementState::Normal:
		GetCharacterMovement()->MaxWalkSpeed = ____DEFAULT_MOVEMENT_SPEED;
		break;

	case EPlayerMovementState::Dashing:
		DashTimer = 0.f;
		GetCharacterMovement()->MaxWalkSpeed = DashSpeed;
		break;

	case EPlayerMovementState::Dead:
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		break;
	}

	PlayerMovementState = NewState;
}

EPlayerMovementState APlayer1::GetPlayerMovementState() const
{
	return PlayerMovementState;
}

void APlayer1::DashButtonPressed()
{
	if (DashTimer == 0.f && GetCharacterMovement()->IsMovingOnGround())
		SetPlayerMovementState(EPlayerMovementState::Dashing);
}

void APlayer1::DashButtonReleased()
{
	if (PlayerMovementState == EPlayerMovementState::Dashing)
	{
		SetPlayerMovementState(EPlayerMovementState::Normal);
		DashTimer = DashCooldown;
	}
}

ASpell* APlayer1::SpawnSpell(TSubclassOf<ASpell> SpellClass)
{
	const FRotator Rotation = GetActorRotation();

	return GetWorld()->SpawnActor<ASpell>(
		SpellClass,
		GetActorLocation() + GetActorForwardVector() * SpellAheadSpawnOffset + SpellSpawnLocationOffset,
		Rotation
		);
}

void APlayer1::NextSpell()
{
	SpellSelector->NextSpell();
}

// Spawn a raincloud with the desired offset
void APlayer1::CastSpell()
{
	// Cast rain cloud spell with offset
	switch (SpellSelector->GetSpellType())
	{
	case ESpellType::Rain:
		SpawnSpell(RainSpellClass);
		break;

	case ESpellType::Wind:
		 SpawnSpell(WindSpellClass);
		break;

	case ESpellType::Shade:
		SpawnSpell(ShadeSpellClass);
		break;
	}
}

// Called every frame
void APlayer1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	

	// Act as STATEd *maniacal laughter*
	switch (PlayerMovementState)
	{
	case EPlayerMovementState::Normal:

		// Dash cooldown
		DashTimer = FMath::Max(DashTimer - DeltaTime, 0.f);

		break;

	case EPlayerMovementState::Dashing:

		DashTimer += DeltaTime;

		if (DashTimer >= DashLength || !GetCharacterMovement()->IsMovingOnGround())
		{
			SetPlayerMovementState(EPlayerMovementState::Normal);
		}

		break;
	}

	// Update the HUD
	if (CurrentHUD && !CurrentHUD->IsPendingKill())
	{
		CurrentHUD->SetHUDSpell(SpellSelector->GetSpellIndex());
	}
	else
	{
		CurrentHUD = nullptr;
	}
}

// Called to bind functionality to input
void APlayer1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	// Set up key press function bindings
	PlayerInputComponent->BindAction("Spell", IE_Pressed, this, &APlayer1::CastSpell);
	PlayerInputComponent->BindAction("NextSpell", IE_Pressed, this, &APlayer1::NextSpell);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &APlayer1::DashButtonPressed);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &APlayer1::DashButtonReleased);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayer1::Pause);
	PlayerInputComponent->BindAction("Quit", IE_Pressed, this, &APlayer1::Quit);
	
	// Set up axis function bindings
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
}

void APlayer1::Pause()
{
	// Pause if able to cast to main game mode
	if (auto GameMode = Cast<AOvercastMainGameMode>(UGameplayStatics::GetGameMode(this)))
		GameMode->TogglePause();
}

void APlayer1::Quit()
{
	if (auto GameMode = Cast<AOvercastGameModeBase>(UGameplayStatics::GetGameMode(this)))
		GameMode->QuitGame();
}
