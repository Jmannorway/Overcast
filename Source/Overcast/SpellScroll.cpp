// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellScroll.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player1.h"
#include "OvercastMainGameMode.h"
#include "OvercastSaveGame.h"
#include "SpellSelector.h"
#include "Components/CapsuleComponent.h"
#include "TriggerComponent.h"

// Sets default values
ASpellScroll::ASpellScroll()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScrollCollisionSphere = CreateDefaultSubobject<USphereComponent>("ScrollCollisionSphere");
	ScrollCollisionSphere->InitSphereRadius(50.f);
	ScrollCollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASpellScroll::ScrollSphereOverlap);
	SetRootComponent(ScrollCollisionSphere);

	ScrollMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScrollMesh");
	ScrollMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ScrollMesh->SetupAttachment(RootComponent);

	SpellToUnlock = ESpellType::Shade;
	ScrollRotationSpeed = 100.f;

	bDestroyOnCollect = true;
	bCollectable = true;

	TriggerComponent = CreateDefaultSubobject<UTriggerComponent>("TriggerComponent");
}

void ASpellScroll::ScrollSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bCollectable)
	{
		// Unlock the spell and disappear
		if (auto Player = Cast<APlayer1>(OtherActor))
		{
			if (auto PlayerCollision = Cast<UCapsuleComponent>(OtherComp))
			{
				Player->SpellSelector->UnlockSpell(SpellToUnlock);
				Player->SpellSelector->SetSpell(SpellToUnlock);
				TriggerComponent->Trigger();
				SetCollectable(false);
			}
		}
	}
}

void ASpellScroll::SetCollectable(bool bNewCollectability)
{
	if (!bNewCollectability && bDestroyOnCollect)
		Destroy();

	bCollectable = false;
	ScrollMesh->SetVisibility(false);
}

// Called every frame
void ASpellScroll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the scroll
	ScrollMesh->AddLocalRotation({ 0.f, ScrollRotationSpeed * DeltaTime, 0.f });
}


