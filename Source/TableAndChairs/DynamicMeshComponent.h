// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "DynamicMeshComponent.generated.h"

/**
 *
 */
UCLASS()
class TABLEANDCHAIRS_API UDynamicMeshComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UDynamicMeshComponent(const FObjectInitializer& ObjectInitializer);

	/**
	 * Calls the function of UPorceduralMesh class to generate the mesh.
	 */
	void GenerateMesh();

	/** Updates the first section of this mesh, using the values of member variables */
	void UpdateMesh();

	/**
	 * Adds the Vertices, Triangles and Normals necessary to create the mesh
	 * @param MeshSize - The size of the Cube
	 * @param Position - Where the mesh will be positioned. NOTE: Its starting pivot will be always (0, 0, 0)
	 */
	void BuildCube(const FVector &MeshSize, const FVector &Position, const FColor &Color);

	/** Resets Vertices, Triangles, Normals, ... */
	void ResetBuffers();

private:

	void BuildQuad(const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &Normal, const FVector &InTangent, const FColor &Color);

	/** The current index of Vertices array */
	int32 VertexIndex;

	/** The current index of Triangles array */
	int32 TrianglesIndex;

	/** The Vertices of this mesh */
	TArray<FVector> Vertices;

	/** The Triangles of this mesh */
	TArray<int32> Triangles;

	/** The Normals of this mesh */
	TArray<FVector> Normals;

	/** The Tangents of this mesh */
	TArray<FProcMeshTangent> Tangents;

	/** The UVs of this mesh */
	TArray<FVector2D> UVs;

	/** The VertexColors of this mesh */
	TArray<FColor> VertexColors;
};
