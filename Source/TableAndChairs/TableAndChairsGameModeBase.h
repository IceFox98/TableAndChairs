// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TableAndChairsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TABLEANDCHAIRS_API ATableAndChairsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	/** Spawns the mesh at Zero position */
	void SpawnMesh() const;

public:

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AActor> ActorToSpawn;
};
