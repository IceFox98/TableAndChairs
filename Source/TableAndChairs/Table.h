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
	 * Creates a table with the size indicated and moves the actor at 'Center' position.
	 * @param Center - The position where the actor will be moved
	 * @param Update - Do you want to update the current mesh section?
	 */
	void BuildMesh(const FVector &Center, const FVector &MeshSize, const bool Update);

	/** The starting size of the table */
	UPROPERTY(EditDefaultsOnly)
		FVector Size;

	/** The maximum size of the table */
	UPROPERTY(EditAnywhere)
		FVector MaxSize;

	/** The minimum size of the table */
	UPROPERTY(EditAnywhere)
		FVector MinSize;

	/**
	 * Resizes the mesh based on the delta and move the actor by half delta
	 * @param Direction - The direction of the resizing (Unit vector)
	 */
	virtual FVector ResizeMesh(const FVector &Direction, const FVector &NewExtent) override;

	/**
	 * Clamps the size between a minimum and maximum value, indicated by the Actor.
	 * Returns the clamped size.
	 * @param Direction - The direction of the resizing (Unit vector)
	 * @param SizeToCheck - The size that must be clamped
	 */
	virtual FVector ClampSize(const FVector &Direction, const FVector &SizeToCheck) override;

	/**
	 * Given the direction and the extent of the resizing, creates an FBox using those 
	 * values and checkes if it intersects with other actors.
	 */
	virtual bool DoesIntersect(const FVector &Direction, const FVector &Extent) override;;

	/** Returns the starting size of the table */
	virtual FVector GetMeshSize() override;

private:

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
};
