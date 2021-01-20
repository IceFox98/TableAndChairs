// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh.h"
#include "TableLeg.generated.h"

/**
 * 
 */
UCLASS()
class TABLEANDCHAIRS_API ATableLeg : public ADynamicMesh
{
	GENERATED_BODY()

public:

	ATableLeg();

	UPROPERTY(EditDefaultsOnly)
		FVector Size = FVector(30.f, 30.f, 100.f);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BuildMesh() override;
	
};
