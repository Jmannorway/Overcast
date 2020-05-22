// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Map.h"
#include "Player1.generated.h"

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
		Spell related variables and functions
	*/

	// Configured rain cloud spell BP instance (will probably be replaced by a more intricate spellcasting system some day)
	UPROPERTY(EditAnywhere, Category = "Spell")
		TSubclassOf<class ARainCloud> RainCloudSpell;

	// Location offset for spells that create a specific object
	UPROPERTY(EditAnywhere, Category = "Spell")
		FVector SpellLocationOffset;

	// How far ahead to of the player to spawn an instance created by a spell
	UPROPERTY(EditAnywhere, Category = "Spell")
		float SpellAheadOffset;

	UFUNCTION()
		void ChangeSpell();

	/*
		The action sphere is the radius in which the player can interact with things
		such as pushable boxes, pickups and, friendly characters
	*/
	UPROPERTY(EditAnywhere, Category = "Action Sphere")
		class USphereComponent* ActionSphere;

	UFUNCTION()
		void OnActionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnActionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void ActionPressed();

	UFUNCTION()
		void ActionReleased();

	/*
		Variables and functions to interact with pushable boxes
	*/

	bool bCanPushBox;

	bool bIsPushingBox;

	UPROPERTY(EditAnywhere, Category = "Pushing")
		float PushingStateSpeed;

	class APushableBox* PushableBox;

	/*
		Camera trigger reference to keep track
	*/

	class ACameraTrigger* CameraTrigger;

	/*
		Other variables & functions
	*/

	void ReportOnStuff();

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

	void Slide();

	void StopSlide();

	void Pause();

	void Quit();

	UFUNCTION(BlueprintCallable, Category = "Spell")
		USpellSelector* GetSpellSelector() const;

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }
	FORCEINLINE ULensmanSpringArmComponent* GetSpringArmComponent() const { return CameraArm; }
};
