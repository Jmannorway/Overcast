// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "RainCloud.h"

// Sets default values
APlantPlatform::APlantPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>("DummyRoot");
	SetRootComponent(DummyRoot);

	// Set up the water-able plant mesh
	PlantMesh = CreateDefaultSubobject<UStaticMeshComponent>("PlantBox");
	PlantMesh->SetupAttachment(RootComponent);

	// Set a platform mesh
	Platform = CreateDefaultSubobject<UBoxComponent>("Platform");
	Platform->SetupAttachment(RootComponent);
	Platform->InitBoxExtent({ 250.f, 250.f, 20.f });
	Platform->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Platform->SetHiddenInGame(false);

	// Set growth default values
	GrowthHeight = 250.f;
	GrowthSpeed = 0.015f;
	GrowthPhase = EGrowthPhase::Idle;
	GrowthProgress = 0.f;
}

// Called when the game starts or when spawned
void APlantPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	PlantMesh->OnComponentBeginOverlap.AddDynamic(this, &APlantPlatform::OnPlantBeginOverlap);
}

void APlantPlatform::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void APlantPlatform::OnPlantBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GrowthPhase == EGrowthPhase::Idle && Cast<ARainCloud>(OtherActor))
	{
		GrowthPhase = EGrowthPhase::Growing;

		// Get the original location of the now-rising platform
		OriginZ = GetActorLocation().Z;
	}
}

void APlantPlatform::UpdatePlatformNumber()
{
	/*const int32 CurrentPlatformNumber = Platform.Num();

	if (Platform.Num() < PlatformNumber)
	{
		Platform.SetNum(PlatformNumber);

		FString ComponentNameString = "Platform";

		for (int32 i = CurrentPlatformNumber - 1; i < PlatformNumber; i++)
		{
			ComponentNameString = "Platform";
			ComponentNameString.AppendInt(i);
			Platform = CreateDefaultSubobject<UBoxComponent>("Platform1");
		}
	}
	else if (Platform.Num() > PlatformNumber)
	{
		for (int32 i = CurrentPlatformNumber - 1; i >= PlatformNumber; i--)
		{
			if (Platform != nullptr)
			{
				Platform->DestroyComponent();
			}
		}

		Platform.SetNum(PlatformNumber);
	}*/
}

// Called every frame
void APlantPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GrowthPhase == EGrowthPhase::Growing)
	{
		GrowthProgress = FMath::Min(GrowthProgress + GrowthSpeed, 1.f);

		float SinLerp = FMath::Sin(GrowthProgress * (PI / 2.f));

		FVector Location = GetActorLocation();
		SetActorLocation({ Location.X, Location.Y, OriginZ + GrowthHeight * SinLerp });

		if (GrowthProgress == 1.f)
			GrowthPhase = EGrowthPhase::Grown;
	}
}

