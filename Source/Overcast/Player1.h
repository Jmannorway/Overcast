// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Map.h"
#include "Player1.generated.h"

class ASpell;

UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	Normal,
	Dashing,
	Dead,
	NUMBER
};

class USpellSelector;

UCLASS()
class OVERCAST_API APlayer1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer1();

	// The spell selector is public to allow for easy editing
	UPROPERTY(EditAnywhere, Category = "Spell")
		USpellSelector* SpellSelector;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Camera boom behind player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class ULensmanSpringArmComponent* CameraArm;

	//Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<APlayer1> PlayerSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PlayerDestroyed;

	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		int32 DeathCon;

	/*
		Sphere component collision functions
	*/

	UFUNCTION()
		void CapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CapsuleComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	/*
		Keep track of movement states
	*/

	UPROPERTY(BlueprintReadOnly, Category = "State")
		EPlayerMovementState PlayerMovementState;

public:

	UFUNCTION(BlueprintCallable, Category = "State")
		void SetPlayerMovementState(EPlayerMovementState NewState);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		EPlayerMovementState GetPlayerMovementState() const;

protected:

	/*
		Dashing functions and variables
	*/

	UFUNCTION()
		void DashButtonPressed();

	UFUNCTION()
		void DashButtonReleased();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		float DashSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		float DashLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
		float DashCooldown;

	UPROPERTY(BlueprintReadOnly, Category = "Dash")
		bool bIsDashing;

	UPROPERTY(BlueprintReadOnly, Category = "Dash")
		float DashTimer;

	/*
		Spell related variables and functions
	*/

	// Class of the rain spell instance to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Spell")
		TSubclassOf<ASpell> RainSpellClass;

	// Class of the wind spell instance to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Spell")
		TSubclassOf<ASpell> WindSpellClass;

	// Class of the shade spell instance to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Spell")
		TSubclassOf<ASpell> ShadeSpellClass;

	// Location offset for spells that create a specific object
	UPROPERTY(EditAnywhere, Category = "Spell")
		FVector SpellSpawnLocationOffset;

	// How far ahead to of the player to spawn an instance created by a spell
	UPROPERTY(EditAnywhere, Category = "Spell")
		float SpellAheadSpawnOffset;

	// Cool down time for a spell
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell")
		float SpellCooldown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spell")
		float SpellCooldownTimer;

	// Spawn spell of class
	UFUNCTION(BlueprintCallable, Category = "Spell")
		ASpell* SpawnSpell(TSubclassOf<ASpell> SpellClass);

	// Cast the selected spell
	UFUNCTION(BlueprintCallable, Category = "Spell")
		void CastSpell();

	// Select the next spell
	UFUNCTION(BlueprintCallable, Category = "Spell")
		void NextSpell();

	/*
		Camera trigger reference to keep track
	*/

	class ACameraTrigger* CameraTrigger;

private:

	class AGameHUD* CurrentHUD;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Called for forward/backward input
	void ForwardMovement(float Value);

	//Called for side to side
	void HorizontalMovement(float Value);

	void Pause();

	void Quit();

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }
	FORCEINLINE ULensmanSpringArmComponent* GetSpringArmComponent() const { return CameraArm; }
};
