// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh.h"
#include "Table.generated.h"

class AChair;
class ATableLeg;
class AResizePoint;

enum class EAxes : uint8
{
	None,
	X,
	Y,
	Z
};

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

	TArray<ATableLeg*> Legs;
	TMap<EAxes, TArray<AChair*>*> ChairsOnAxis;

public:

	UPROPERTY(VisibleAnywhere)
		TArray<AResizePoint*> ResizePoints;

	UPROPERTY(EditDefaultsOnly)
		int32 ResizePointsCount = 4;

	UPROPERTY(EditDefaultsOnly)
		int32 LegsCount = 4;

private:
	APlayerController* PlayerController;

	bool bRecordingMovement;
	FVector StartHitPoint;
	FVector StartCenter;
	TArray<FVector> StartVertices;

	const float DistanceFromTable = .25f;
	float ChairOffset;
	float ChairWidth;

	void SpawnResizePoints();
	void SpawnTableLegs();
	void SetupInputBinding();

	void StartRecordingMovement();
	void StopRecordingMovement();

	/**
	 * Updates the Mesh of the table according to the point hit by mouse.
	 * @param MovementAmount - The total movement tracked by the mouse.
	 */
	void UpdateTableMesh(const FVector &MovementAmount);

	/** Updates the table's legs position according to table's bounds. */
	void UpdateLegsTransform();

	/** Updates the resize points' position according to table's bounds. */
	void UpdateResizePointsTransform();

	/**  */
	void UpdateTransforms(TArray<AActor*> ActorsToUpdate, const TArray<FVector> &NewPositions);

	FVector GetTableLegSize() const;

	void CalculateChairs();
	float GetTotalChairLength(const int ChairsPerSide, const float TableSideLength) const;
	void CalculateChairsOfAxis(FVector StartSpawnPoint, FVector SpawnOffset, const int ChairsPerSide, const float TotalChairLength, const EAxes FlipAxis);
	void GenerateChairsFromPoint(const FVector &StartSpawnPoint, float InitialRotation, const EAxes FlipAxis);
	void FixCharirTransform(AChair &Chair, FVector StartSpawnPoint, FVector SpawnOffset, float Yaw, const EAxes FlipAxis);
};
