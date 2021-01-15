// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh.h"
#include "Table.generated.h"

class AChair;
class ATableLeg;
class AResizePoint;

UCLASS()
class TABLEANDCHAIRS_API ATable : public ADynamicMesh
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void BuildMesh() override;

	TArray<AChair*> Chairs;
	TArray<ATableLeg*> Legs;

public:

	UPROPERTY(VisibleAnywhere)
		TArray<AResizePoint*> ResizePoints;

	UPROPERTY(EditDefaultsOnly)
		int32 ResizePointsCount = 0;

private:
	APlayerController* PlayerController;

	bool bRecordingMovement;
	FVector StartHitPoint;
	FVector StartCenter;

	TArray<FVector> StartVertices;

	void StartRecordingMovement();
	void StopRecordingMovement();

	void UpdateTableMesh(const FVector &MovementAmount);
	void UpdateLegsTransform();
	void UpdateResizePointsTransform();

	void UpdateTransforms(TArray<AActor*> ActorsToUpdate, const TArray<FVector> &NewPositions);

	void CalculateChairs();
};
