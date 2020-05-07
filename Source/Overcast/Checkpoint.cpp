// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OvercastGameMode.h"
#include "Components/BillboardComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OffsetComponent = CreateDefaultSubobject<USceneComponent>("OffsetComponent");
	SetRootComponent(OffsetComponent);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Ghost");
	Mesh->SetupAttachment(RootComponent);

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->InitBoxExtent({ 200.f, 200.f, 200.f });
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnBoxBeginOverlap);
	Box->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	Sprite = CreateDefaultSubobject<UBillboardComponent>("Sprite");
	Sprite->SetupAttachment(RootComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->ArrowColor = FColor::Red;
	Arrow->SetRelativeLocation(SpawnLocationOffset);
	Arrow->SetupAttachment(Sprite);
#endif // WITH_EDITORONLY_DATA
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACheckpoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	Sprite->SetRelativeLocation(SpawnLocationOffset);
	Arrow->SetWorldRotation(SpawnRotation);
#endif // WITH_EDITORONLY_DATA
}

int32 ACheckpoint::GetCheckpointIndex() const
{
	return CheckpointIndex;
}

FVector ACheckpoint::GetSpawnLocation() const
{
	return GetActorLocation() + SpawnLocationOffset;
}

FRotator ACheckpoint::GetSpawnRotation() const
{
	return SpawnRotation;
}

void ACheckpoint::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == (void*)UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		if (auto Gamemode = Cast<AOvercastGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			Gamemode->SetCheckpointIndex(CheckpointIndex);
			UE_LOG(LogTemp, Warning, TEXT("Set checkpoint"));
		}
	}
}
