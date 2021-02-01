// Fill out your copyright notice in the Description page of Project Settings.


#include "TableAndChairsGameModeBase.h"

void ATableAndChairsGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnMesh();
}

void ATableAndChairsGameModeBase::SpawnMesh()
{
	//Pointer cannot be reassigned, but World can still be modified
	UWorld* const World = GetWorld();

	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is NULL!"));
		return;
	}

	if (!ActorToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("ActorToSpawn is NULL!"));
		return;
	}

	World->SpawnActor<AActor>(ActorToSpawn);
}