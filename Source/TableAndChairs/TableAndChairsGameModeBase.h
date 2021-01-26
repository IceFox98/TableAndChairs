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

public:
	
	UPROPERTY(EditDefaultsOnly)
		FVector TableSize = FVector(200.f, 200.f, 20.f);

	UPROPERTY(EditDefaultsOnly)
		FVector TableLegSize = FVector(30.f, 30.f, 100.f);

	UPROPERTY(EditDefaultsOnly)
		FVector ChairSeatSize = FVector(60.f, 60.f, 10.f);

	UPROPERTY(EditDefaultsOnly)
		FVector ChairBackSize = FVector(60.f, 15.f, 80.f);

	UPROPERTY(EditDefaultsOnly)
		FVector ChairLegSize = FVector(15.f, 15.f, 70.f);
};
