// Copyright Epic Games, Inc. All Rights Reserved.


#include "TableAndChairsGameModeBase.h"
#include "GameFramework/Actor.h"
#include "Table.h"


void ATableAndChairsGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Begin Play"));

	GenerateTableAndChairs();

}

void ATableAndChairsGameModeBase::GenerateTableAndChairs()
{

	UWorld* world = GetWorld();

	if (world)
	{
		world->SpawnActor<ADynamicMesh>(ATable::StaticClass());
	}


}

