// Fill out your copyright notice in the Description page of Project Settings.


#include "RainSpell.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Player1.h"



// Sets default values
ARainSpell::ARainSpell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpellCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("SpellCollider"));
	SpellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpellMesh"));
	SpellCollider->OnComponentBeginOverlap.AddDynamic(this, &ARainSpell::Overlap);

	RootComponent = SpellCollider;
	SpellMesh->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ARainSpell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARainSpell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARainSpell::Overlap(UPrimitiveComponent* Overlapcomp, AActor* OtherActor, UPrimitiveComponent* Othercomp, int32 OtherBodyIndex, bool bFSweep, const FHitResult& SweepR)
{
	if (OtherActor->IsA(APlayer1::StaticClass()))
	{

		Destroy();
	}
}
