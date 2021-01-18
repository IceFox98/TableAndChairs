// Copyright Epic Games, Inc. All Rights Reserved.


#include "TableAndChairsGameModeBase.h"
#include "GameFramework/Actor.h"
#include "Table.h"
#include "PlayerCamera.h"
#include "Chair.h"


void ATableAndChairsGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnMesh();
}

void ATableAndChairsGameModeBase::SpawnMesh()
{
	//Pointer cannot be reassigned, but World can still be modified
	UWorld* const World = GetWorld();

	if (World)
	{
		World->SpawnActor<ADynamicMesh>(ATable::StaticClass());
	}


}

