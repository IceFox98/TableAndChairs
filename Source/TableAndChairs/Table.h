// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh.h"
#include "Table.generated.h"

class AChair;
class ATableLeg;
class AResizePoint;

UENUM()
enum class EAxes : uint8
{
	None,
	X,
	Y,
	Z
};

USTRUCT()
struct FChairs
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere)
		TArray<AChair*> Chairs;

	FChairs()
	{
		Chairs = TArray<AChair*>();
	}
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

public:

	UPROPERTY(EditDefaultsOnly)
		FVector Size = FVector(200.f, 200.f, 20.f);

private:

	UPROPERTY()
		int32 LegsCount = 4;

	UPROPERTY(VisibleAnywhere)
		TArray<ATableLeg*> Legs;

	UPROPERTY()
		int32 ResizePointsCount = 4;

	UPROPERTY(VisibleAnywhere)
		TArray<AResizePoint*> ResizePoints;

	/** Contains chairs divided by which axis has been flipped */
	UPROPERTY(VisibleAnywhere)
		TMap<EAxes, FChairs> ChairsOnAxis;

	APlayerController* PlayerController;

	bool bRecordingMovement = false;

	FVector StartHitPoint;
	FVector StartCenter;

	TArray<FVector> StartVertices;

	/** How far chair is from the side of the table */
	UPROPERTY(EditDefaultsOnly, Category = "Chair Info", meta = (AllowPrivateAccess = "true"))
		float DistanceFromTable = .25f;

	/** The offset for each chair's side */
	UPROPERTY(EditDefaultsOnly, Category = "Chair Info", meta = (AllowPrivateAccess = "true"))
		float ChairOffset = 15.f;

	/** Chair width + ChairOffset for each side */
	float ChairWidthWithOffset;

	/** How far chair is form the bottom side of the table */
	float ChairOffsetZ;

	/** Generates Resize Points and attaches them as children of Table actor */
	void SpawnResizePoints();

	/**  */
	void SpawnTableLegs();

	/**  */
	void SetupInputBinding();

	/**  */
	void StartRecordingMovement();

	/**  */
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

	/**
	 * Given an array of Actor and one of FVector (positions), having the same size, it updates each Actor
	 * location with the relative position. Actors[0] location will be set to Positions[0].
	 */
	void UpdateTransforms(TArray<AActor*> ActorsToUpdate, const TArray<FVector> &NewPositions);

	/** Returns the size of the table leg */
	FVector GetTableLegSize() const;

	/** Returns the size of the seat of the chair */
	FVector GetChairSeatSize() const;

	/** Calculates the number of chairs for each table's side and generates/removes/updates them. */
	void CalculateChairs();

	/**
	 * Calculates the remaining space between the whole chairs length (offset included) and table length.
	 * Then adds it to chair width.
	 * @param ChairsPerSide - The number of chairs for each opposite table's side
	 * @param TableSideLength - The length of the affected table's side
	 */
	float GetTotalChairLength(const int ChairsPerSide, const float TableSideLength) const;

	/**
	 * Creates or deletes chairs and updates their position according to table size.
	 * @param StartSpawnPoint -
	 * @param SpawnOffset - The offset that will be added to the StartSpawnPoint
	 * @param ChairsPerSide - The number of chairs for each opposite table's side
	 * @param TotalChairLength - The length of the chair, offset included
	 * @param FlipAxis - The axis of the SpawnOffset point which will be multiplied by -1
	 */
	void CalculateChairsOfAxis(FVector StartSpawnPoint, FVector SpawnOffset, const int ChairsPerSide, const float TotalChairLength, const EAxes FlipAxis);

	/**
	 * Spawns one chairs for each side of table (only the opposite ones)
	 * @param FlipAxis -
	 */
	void SpawnChairs(const EAxes FlipAxis);

	/**
	 * Updates the chair's transform and, if specified, flips its offset according to indicated axis.
	 * @param Chair - The Chair actor you want to update
	 * @param StartSpawnPoint -
	 * @param SpawnOffset - The offset that will be added to the StartSpawnPoint
	 * @param Yaw - The rotation on Z Axis you want to apply to Chair
	 * @param FlipAxis - The axis of the SpawnOffset point which will be multiplied by -1
	 */
	void FixChairTransform(AChair &Chair, FVector StartSpawnPoint, FVector SpawnOffset, float Yaw, const EAxes FlipAxis) const;
};
