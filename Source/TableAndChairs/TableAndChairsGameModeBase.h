// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TableAndChairsGameModeBase.generated.h"

class ADynamicMesh;

/**
 *
 */
UCLASS()
class TABLEANDCHAIRS_API ATableAndChairsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	void SpawnMesh();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ADynamicMesh> MeshToSpawn;
};
