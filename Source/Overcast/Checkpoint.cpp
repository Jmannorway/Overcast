// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OvercastGameMode.h"
#include "Components/LineBatchComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->InitBoxExtent({ 50.f, 50.f, 50.f });
	Box->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnBoxBeginOverlap);

#if WITH_EDITORONLY_DATA
	Draw = CreateDefaultSubobject<ULineBatchComponent>("Draw");

	Arrow = CreateDefaultSubobject<UArrowComponent>("Arrow");
	Arrow->ArrowColor = FColor::Red;
	Arrow->SetRelativeLocation(Location);
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

	auto SpawnLocation = GetSpawnLocation();

	Draw->Flush();
	Draw->DrawPoint(SpawnLocation, FColor::Red, 20.f, 0);

	Arrow->SetWorldLocation(SpawnLocation);
	Arrow->SetWorldRotation(Rotation);
#endif // WITH_EDITORONLY_DATA
}

int32 ACheckpoint::GetCheckpointIndex() const
{
	return Index;
}

FVector ACheckpoint::GetSpawnLocation() const
{
	return GetActorLocation() + Location;
}

FRotator ACheckpoint::GetSpawnRotation() const
{
	return Rotation;
}

void ACheckpoint::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == (void*)UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		if (auto Gamemode = Cast<AOvercastGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			Gamemode->CurrentCheckpointIndex = Index;
			UE_LOG(LogTemp, Warning, TEXT("Set checkpoint"));
		}
	}
}
