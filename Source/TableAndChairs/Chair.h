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

	FVector SeatSize = FVector(60.f, 60.f, 10.f);
	FVector BackSize = FVector(60.f, 15.f, 80.f);
	FVector LegSize = FVector(15.f, 15.f, 70.f);
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BuildMesh() override;

};
