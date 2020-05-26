// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingLightningProjectile.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AHomingLightningProjectile::AHomingLightningProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileStart = CreateDefaultSubobject<USceneComponent>("ProjectileStart");
	SetRootComponent(ProjectileStart);

	ProjectileEnd = CreateDefaultSubobject<USceneComponent>("ProjectileEnd");
	ProjectileEnd->SetupAttachment(RootComponent);

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
}

// Called when the game starts or when spawned
void AHomingLightningProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHomingLightningProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

