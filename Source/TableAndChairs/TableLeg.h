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

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BuildMesh() override;
	
	UPROPERTY(VisibleAnywhere)
		FVector Size;
};
