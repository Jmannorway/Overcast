// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Player1.h"

// Sets default values
AProjectileEnemy::AProjectileEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SetRootComponent(Mesh);

	VisionBox = CreateDefaultSubobject<UBoxComponent>("VisionSphere");
	VisionBox->SetupAttachment(RootComponent);

	ChargeLength = 2.f;
}

// Called when the game starts or when spawned
void AProjectileEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectileEnemy::OnVisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<APlayer1>(OtherActor);

	if (Player)
	{
		bInView = true;
		ChargeTimer = 0.f;
	}
}

void AProjectileEnemy::OnVisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == Player)
	{
		bInView = false;
		Player = nullptr;
	}
}

// Called every frame
void AProjectileEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInView)
	{
		ChargeTimer += DeltaTime;

		if (ChargeTimer >= ChargeLength)
		{

		}
	}
}

