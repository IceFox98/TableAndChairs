// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DynamicMeshLibrary.h"
#include "ChairsManager.generated.h"


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
		TArray<UProceduralMeshComponent*> Chairs;

	FChairs()
	{
		Chairs = TArray<UProceduralMeshComponent*>();
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TABLEANDCHAIRS_API UChairsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UChairsManager();

public:	

	/**
	 * Initializes some member variables to calculate chairs' position.
	 * @param MeshLegSize - The size of the parent leg
	 * @param ParentActor - The Actor whose chairs will be attached to
	 */
	void Initialize(const FVector &MeshLegSize, USceneComponent* ParentComp);

	/** Updates the chairs' position depending on the size of the parent mesh */
	void UpdateChairs(const FVector &MeshSize);
		
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector ChairSeatSize;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector ChairBackSize;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector ChairLegSize;

	/** Contains chairs divided by which axis has been flipped */
	UPROPERTY(VisibleAnywhere)
		TMap<EAxes, FChairs> ChairsOnAxis;

	/** How far chair is from the side of the table */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		float DistanceFromMesh;

	/** The offset for each chair's side */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		float ChairOffset;

	/** Chair width + ChairOffset for each side */
	float ChairWidthWithOffset;

	/** How far chair is form the bottom side of the table */
	float ChairOffsetZ;

	FVector ParentLegSize;
	USceneComponent* Parent;

	FProceduralMeshData ChairMeshData;

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
	 */
	void SpawnChair(const EAxes FlipAxis);

	/**
	 * Searches for an available chair (which is visible in game) and returns it.
	 * If it doesn't find any, returns nullptr.
	 */
	UProceduralMeshComponent* GetPooledChair(const EAxes FlipAxis) const;

	/** Sets as non-visible the first chair (on the indicated axis) which is visible. */
	void SetPooledChair(const EAxes FlipAxis);

	/** Returns the total count of visible chairs of the indicated axis */
	int32 GetAvailableChairsCount(const EAxes FlipAxis) const;

	/**
	 * Updates the chair's transform and, if specified, flips its offset according to indicated axis.
	 * @param Chair - The Chair actor you want to update
	 * @param StartSpawnPoint -
	 * @param SpawnOffset - The offset that will be added to the StartSpawnPoint
	 * @param Yaw - The rotation on Z Axis you want to apply to Chair
	 * @param FlipAxis - The axis of the SpawnOffset point which will be multiplied by -1
	 */
	void FixChairTransform(UProceduralMeshComponent &Chair, FVector StartSpawnPoint, FVector SpawnOffset, float Yaw, const EAxes FlipAxis) const;
};
