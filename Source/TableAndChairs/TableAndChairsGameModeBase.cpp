// Copyright Epic Games, Inc. All Rights Reserved.


#include "TableAndChairsGameModeBase.h"
#include "GameFramework/Actor.h"
#include "Table.h"


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

	if (!MeshToSpawn)
	{
		UE_LOG(LogTemp, Error, TEXT("MeshToSpawn is NULL!"));
		return;
	}

	World->SpawnActor<ADynamicMesh>(MeshToSpawn);
}

