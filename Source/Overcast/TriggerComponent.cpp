// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StaticMeshTrigger.h"

// Sets default values for this component's properties
UTriggerComponent::UTriggerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bTriggerOnDestroy = false;
}


void UTriggerComponent::Trigger()
{
	TArray<AActor*> TriggerableActor;
	UGameplayStatics::GetAllActorsOfClass(this, TriggerableClass, TriggerableActor);

	AStaticMeshTrigger* CastedTriggerableActor;

	UE_LOG(LogTemp, Warning, TEXT("This code is excecuted"));

	for (int32 i = 0; i < TriggerableActor.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("This code is also excecuted"));

		CastedTriggerableActor = CastChecked<AStaticMeshTrigger>(TriggerableActor[i]);

		if (CastedTriggerableActor->GetTriggerIndex() == TriggerIndex)
		{
			CastedTriggerableActor->Trigger();

			UE_LOG(LogTemp, Warning, TEXT("Even this code is excecuted"));
		}
	}
			
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTriggerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (bTriggerOnDestroy)
		Trigger();
}


// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

