// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Player1.h"

// Sets default values
ALightningProjectile::ALightningProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KillSphere = CreateDefaultSubobject<USphereComponent>("KillSphere");
	KillSphere->InitSphereRadius(32.f);
	KillSphere->OnComponentBeginOverlap.AddDynamic(this, &ALightningProjectile::OnKillSphereBeginOverlap);
	SetRootComponent(KillSphere);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystemComponent");
	ParticleSystem->SetupAttachment(RootComponent);

	ProjectileSpeed = 240.f;
}

// Called when the game starts or when spawned
void ALightningProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the beams
	if (GetInstigator())
	{
		ParticleSystem->SetActorParameter("Source", GetInstigator());
	}
	else
	{
		Destroy();
		UE_LOG(LogTemp, Warning, TEXT("No instigator"));
	}

	ParticleSystem->SetActorParameter("Target", this);
}

void ALightningProjectile::OnKillSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto Player = Cast<APlayer1>(OtherActor))
	{
		Player->SetPlayerMovementState(EPlayerMovementState::Dead);
	}
}

// Called every frame
void ALightningProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move towards player
	if (TargetActor && !TargetActor->IsPendingKill() && !GetInstigator()->IsPendingKill())
	{
		// Increase the attack radius by the set speed
		CurrentProjectileRange += ProjectileSpeed * DeltaTime;

		// Get values for calculation
		const FVector InstigatorLocation = GetInstigator()->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();
		const float TargetActorDistance = FVector::Distance(InstigatorLocation, TargetLocation);

		// Calculate direction vector
		FVector DirectionVector = InstigatorLocation - TargetLocation;
		DirectionVector.Normalize();

		// ...
		SetActorLocation(InstigatorLocation + DirectionVector * FMath::Min(CurrentProjectileRange, TargetActorDistance));
	}
	else
	{
		Destroy();
	}
}

