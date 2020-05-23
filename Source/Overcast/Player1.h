// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Map.h"
#include "Player1.generated.h"

UENUM(BlueprintType)
enum class EPlayerMovementState : uint8
{
	Normal,
	Dashing,
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

	UFUNCTION()
		void SetPlayerMovementState(EPlayerMovementState NewState);

public:
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

	// Configured rain cloud spell BP instance (will probably be replaced by a more intricate spellcasting system some day)
	UPROPERTY(EditAnywhere, Category = "Spell")
		TSubclassOf<class ARainCloud> RainSpellClass;

	// Location offset for spells that create a specific object
	UPROPERTY(EditAnywhere, Category = "Spell")
		FVector SpellLocationOffset;

	// How far ahead to of the player to spawn an instance created by a spell
	UPROPERTY(EditAnywhere, Category = "Spell")
		float SpellAheadOffset;

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

	void Spell();

	void Pause();

	void Quit();

	void NextSpell();

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }
	FORCEINLINE ULensmanSpringArmComponent* GetSpringArmComponent() const { return CameraArm; }
};
