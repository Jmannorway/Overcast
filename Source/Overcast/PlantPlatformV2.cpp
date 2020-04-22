#include "PlantPlatformV2.h"
#include "Components/StaticMeshComponent.h"
#include "RainCloud.h"

APlantPlatformV2::APlantPlatformV2()
{
	PrimaryActorTick.bCanEverTick = true;

	DummyRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(DummyRoot);

	TopPlant = CreateDefaultSubobject<UStaticMeshComponent>("TopPlant");
	TopPlant->SetupAttachment(RootComponent);
	TopPlant->SetGenerateOverlapEvents(true);

	BottomPlant = CreateDefaultSubobject<UStaticMeshComponent>("BottomPlant");
	BottomPlant->SetupAttachment(RootComponent);

	TopPlantSpeed = 0.06f;
	BottomPlantSpeed = 0.02f;

	GrowingPhase = EGrowingPhase::Idle;
	GrowingProgress = 0.f;
}

void APlantPlatformV2::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &APlantPlatformV2::RainBeginOverlap);

	if (TopPlant)
		TopPlantHeight = TopPlant->Bounds.BoxExtent.Z;

	if (BottomPlant)
		BottomPlantHeight = BottomPlant->Bounds.BoxExtent.Z;
}

void APlantPlatformV2::RainBeginOverlap(AActor* OverlappingActor, AActor* OtherActor)
{
	if (GrowingPhase == EGrowingPhase::Idle && Cast<ARainCloud>(OtherActor))
		AdvancePhase();
}

void APlantPlatformV2::AdvancePhase()
{
	switch (GrowingPhase)
	{
	case EGrowingPhase::Idle: GrowingPhase = EGrowingPhase::TopPlant; UE_LOG(LogTemp, Warning, TEXT("FGFFF")) break;
	case EGrowingPhase::TopPlant: GrowingPhase = EGrowingPhase::BottomPlant; break;
	case EGrowingPhase::BottomPlant: GrowingPhase = EGrowingPhase::Grown; break;
	}

	GrowingProgress = 0.f;
}

void APlantPlatformV2::Tick(float DeltaTime)
{
	if (GrowingPhase == EGrowingPhase::TopPlant)
	{
		GrowingProgress = FMath::Min(GrowingProgress + TopPlantSpeed, 1.f);
		TopPlant->AddLocalOffset(FVector(0.f, 0.f, -TopPlantSpeed * TopPlant->Bounds.BoxExtent.Z));

		if (GrowingProgress == 1.f)
			AdvancePhase();
	}
	else if (GrowingPhase == EGrowingPhase::BottomPlant)
	{
		GrowingProgress = FMath::Min(GrowingProgress + BottomPlantSpeed, 1.f);
		BottomPlant->AddLocalOffset(FVector(0.f, 0.f, BottomPlantSpeed * BottomPlant->Bounds.BoxExtent.Z * 2.0));

		if (GrowingProgress == 1.f)
			AdvancePhase();
	}
}
