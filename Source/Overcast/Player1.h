// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Player1.generated.h"

UCLASS()
class OVERCAST_API APlayer1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer1();

	//Camera boom behind player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	//Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	// Configured rain cloud spell BP instance (will probably be replaced by a more intricate spellcasting system some day)
	UPROPERTY(EditAnywhere = "Spell")
		TSubclassOf<class ARainCloud> RainCloudSpell;

	// Location offset for spells that create a specific object
	UPROPERTY(EditAnywhere, Category = "Spell")
		FVector SpellLocationOffset;

	// How far ahead to of the player to spawn an instance created by a spell
	UPROPERTY(EditAnywhere, Category = "Spell")
		float SpellAheadOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Called for forward/backward input
	void VerticalMovement(float Value);

	//Called for side to side
	void HorizontalMovement(float Value);

	void Spell();

	void Slide();

	void StopSlide();



	FORCEINLINE	class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


};
