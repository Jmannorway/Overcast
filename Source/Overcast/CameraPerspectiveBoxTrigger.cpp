// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPerspectiveBoxTrigger.h"
#include "Player1.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ACameraPerspectiveBoxTrigger::ACameraPerspectiveBoxTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Default values
	bOnBeginOverlap = true;
	bOnEndOverlap = true;

	// Set up trigger box
	TriggerBox = CreateDefaultSubobject<UBoxComponent>("BoxTrigger");
	SetRootComponent(TriggerBox);
	TriggerBox->InitBoxExtent({ 80.f, 80.f, 80.f });
	TriggerBox->ShapeColor = FColor::Green;

#if WITH_EDITORONLY_DATA
	SpriteComponent = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Sprite");
	SpriteComponent->SetupAttachment(RootComponent);
#endif
}

void ACameraPerspectiveBoxTrigger::OnPlayerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	// Casting like this seems to work as a type check. Probably inefficient though
	APlayer1* Player = Cast<APlayer1>(OtherActor);

	if (Player)
		UE_LOG(LogTemp, Warning, TEXT("Begin"));
}

void ACameraPerspectiveBoxTrigger::OnPlayerEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

}

// Called when the game starts or when spawned
void ACameraPerspectiveBoxTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (bOnBeginOverlap)
		OnActorBeginOverlap.AddDynamic(this, &ACameraPerspectiveBoxTrigger::OnPlayerBeginOverlap);

	if (bOnEndOverlap)
		OnActorEndOverlap.AddDynamic(this, &ACameraPerspectiveBoxTrigger::OnPlayerEndOverlap);
}

