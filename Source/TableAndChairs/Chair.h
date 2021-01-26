// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh.h"
#include "Chair.generated.h"

/**
 *
 */
UCLASS()
class TABLEANDCHAIRS_API AChair : public ADynamicMesh
{
	GENERATED_BODY()

public:

	AChair();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BuildMesh() override;

	UPROPERTY(VisibleAnywhere)
		FVector SeatSize;

	UPROPERTY(VisibleAnywhere)
		FVector BackSize;

	UPROPERTY(VisibleAnywhere)
		FVector LegSize;
};
