// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelStreamerTrigger.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"


ALevelStreamerTrigger::ALevelStreamerTrigger()
{
	GetCollisionComponent()->ShapeColor = FColor::Purple;
}

void ALevelStreamerTrigger::BeginPlay()
{
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ALevelStreamerTrigger::TriggerBeginOverlap);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &ALevelStreamerTrigger::TriggerEndOverlap);
}

void ALevelStreamerTrigger::TriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (Player && Player == OtherActor && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	}
}

void ALevelStreamerTrigger::TriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (Player && Player == OtherActor && LevelToUnload != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, LatentInfo, false);
	}
}