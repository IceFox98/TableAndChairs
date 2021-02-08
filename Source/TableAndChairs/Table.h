// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResizableObject.h"
#include "Table.generated.h"

class UDynamicMeshComponent;
class UResizePointManager;
class ULegsManager;
class UChairsManager;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	virtual FVector ResizeMesh(const FVector &Direction, const FVector &DeltaSize) override;

	/**
	 * Clamps the size between a minimum and maximum value, indicated by the Actor.
	 * Returns the clamped size.
	 * @param Direction - The direction of the resizing (Unit vector)
	 * @param SizeToCheck - The size that must be clamped
	 */
	virtual FVector ClampSize(const FVector &Direction, const FVector &SizeToCheck) override;

private:

	UPROPERTY(VisibleAnywhere)
		UDynamicMeshComponent* DynamicMeshComponent;

	UPROPERTY(VisibleAnywhere)
		UResizePointManager* ResizePointManager;

	UPROPERTY(VisibleAnywhere)
		ULegsManager* LegsManager;

	UPROPERTY(VisibleAnywhere)
		UChairsManager* ChairsManager;
};
