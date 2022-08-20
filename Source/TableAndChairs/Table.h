// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResizableObject.h"
#include "Table.generated.h"

UCLASS()
class TABLEANDCHAIRS_API ATable : public AActor, public IResizableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/**
	 * Resizes the mesh based on the delta and move the actor by half delta
	 * @param Direction - The direction of the resizing (Unit vector)
	 * @param NewExtent - The new extent of the mesh
	 */
	virtual FVector ResizeMesh(const FVector &Direction, const FVector &NewExtent) override;

	/**
	 * Clamps the size between a minimum and maximum value, indicated by the Actor.
	 * @param Direction - The direction of the resizing (Unit vector)
	 * @param OutSizeToClamp - The size that must be clamped
	 */
	virtual void ClampSize(const FVector &Direction, FVector &OutSizeToClamp) const override;

	/**
	 * Given the direction and the extent of the resizing, creates an FBox using those 
	 * values and checkes if it intersects with other actors.
	 * @param Direction - The direction of the resizing (Unit vector)
	 * @param Extent - The supposed extent the table will have
	 */
	virtual bool DoesIntersect(const FVector &Direction, const FVector &Extent) override;

	/** Returns the starting size of the table */
	virtual FVector GetMeshSize() const override;

private:

	/** The starting size of the table */
	UPROPERTY(EditDefaultsOnly)
		FVector Size;

	/** The maximum size of the table */
	UPROPERTY(EditAnywhere)
		FVector MaxSize;

	/** The minimum size of the table */
	UPROPERTY(EditAnywhere)
		FVector MinSize;
	
	/** The material applied to procedural mesh */
	UPROPERTY(EditDefaultsOnly)
		UMaterial* MeshMaterial;

	/** The current size of the mesh */
	FVector CurrentSize;

	UPROPERTY(VisibleAnywhere)
		class UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(VisibleAnywhere)
		class UResizePointManager* ResizePointManager;

	UPROPERTY(VisibleAnywhere)
		class ULegsManager* LegsManager;

	UPROPERTY(VisibleAnywhere)
		class UChairsManager* ChairsManager;

	/**
	 * Creates a table with the size indicated and moves the actor at 'Center' position.
	 * @param Center - The position where the actor will be moved
	 * @param MeshSize - The size of the mesh you want to generate
	 * @param Update - Do you want to update the current mesh section?
	 */
	void BuildMesh(const FVector &Center, const FVector &MeshSize, const bool Update);

	/**
	 * Calculates the new center of the table, which delta will be the half of DeltaSize.
	 * @param Direction - The direction of the resizing (Unit vector)
	 * @param DeltaSize - How much the table should be resized.
	 */
	FVector CalculateNewCenter(const FVector &Direction, const FVector &DeltaSize) const;
};
