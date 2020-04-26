// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Map.h"
#include "Player1.generated.h"

UENUM(BlueprintType)
enum class ESpellType : uint8
{
	Rain	UMETA(DisplayName = "Rain"),
	Wind	UMETA(DisplayName = "Wind"),
	Shade	UMETA(DisplayName = "Shade"),
	NUMBER,
	INVALID
};

UCLASS(BlueprintType)
class USpellSelector : public UObject
{
	GENERATED_BODY()

private:
	union { ESpellType t; uint8 i; } mSpell;
	static const uint8 mNumber = static_cast<uint8>(ESpellType::NUMBER);
	bool mbKeychain[mNumber];

public:
	ESpellType IndexToType(uint8 SpellIndex);
	uint8 TypeToIndex(ESpellType SpellType);

	UFUNCTION(BlueprintCallable) ESpellType GetSpellType() const;
	UFUNCTION(BlueprintCallable) uint8 GetSpellIndex() const;

	UFUNCTION(BlueprintCallable) void SetSpell(ESpellType SpellType);
	void SetSpell(uint8 SpellIndex);

	UFUNCTION(BlueprintCallable) void UnlockSpell(ESpellType SpellType);
	void UnlockSpell(uint8 SpellIndex);

	USpellSelector();
	
};

UCLASS()
class OVERCAST_API APlayer1 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayer1();

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

	UPROPERTY(EditAnywhere, Category = "Spell")
		USpellSelector* SpellSelector;

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

	UFUNCTION(BlueprintCallable, Category = "Spell")
		USpellSelector* GetSpellSelector() const;

	FORCEINLINE UCameraComponent* GetCameraComponent() const { return Camera; }
	FORCEINLINE ULensmanSpringArmComponent* GetSpringArmComponent() const { return CameraArm; }
};
