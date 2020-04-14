// Fill out your copyright notice in the Description page of Project Settings.


#include "PushableBox.h"
#include "Components/StaticMeshComponent.h"

APushableBox::APushableBox()
{
	// Make the block detectable
	UStaticMeshComponent* Mesh = GetStaticMeshComponent();
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetSimulatePhysics(true);
}