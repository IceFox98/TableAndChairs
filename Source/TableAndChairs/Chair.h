// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshComponent.h"
#include "Chair.generated.h"

/**
 * 
 */
UCLASS()
class TABLEANDCHAIRS_API UChair : public UDynamicMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChair(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Creates a table with the sizes indicated */
	void BuildMesh(const FVector &SeatSize, const FVector &BackSize, const FVector &LegSize);
};
