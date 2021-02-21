// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "DynamicMeshLibrary.generated.h"

//I use this struct because the input of UProceduralMeshComponent suxx
USTRUCT()
struct FProceduralMeshData
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere)
		TArray<FVector> Vertices;

	UPROPERTY(VisibleAnywhere)
		TArray<int32> Triangles;

	UPROPERTY(VisibleAnywhere)
		TArray<FVector> Normals;

	UPROPERTY(VisibleAnywhere)
		TArray<FProcMeshTangent> Tangents;

	UPROPERTY(VisibleAnywhere)
		TArray<FVector2D> UVs;

	UPROPERTY(VisibleAnywhere)
		TArray<FColor> VertexColors;

	FProceduralMeshData()
		: Vertices(TArray<FVector>())
		, Triangles(TArray<int32>())
		, Normals(TArray<FVector>())
		, Tangents(TArray<FProcMeshTangent>())
		, UVs(TArray<FVector2D>())
		, VertexColors(TArray<FColor>())
	{}

};

UCLASS()
class TABLEANDCHAIRS_API UDynamicMeshLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/**
	 * Buils a cube and fills the outer structure.
	 * @param OutMeshData - The structure that contains the data to create the final mesh
	 * @param Size - The size of the Cube
	 * @param Position - Where the mesh will be positioned. NOTE: Its starting pivot will be always (0, 0, 0)
	 * @param Color - The color of the cube
	 */
	static void BuildCube(FProceduralMeshData &OutMeshData, const FVector &Size, const FVector &Position, const FColor &Color);

private:

	/**
	 * Using the input parameters, builds a quad and fills the outer struct with the newly generated data.
	 * @param OutMeshData - The structure that contains the data to create the final mesh
	 * @param BottomLeft - The position of the bottom left corner
	 * @param BottomRight - The position of the bottom right corner
	 * @param TopRight - The position of the top right corner
	 * @param TopLeft - The position of the top left corner
	 * @param InNormal - The normal of the quad, each corner will have the same normal
	 * @param InTangent - The tangent of the quad, each corner will have the same tangent
	 * @param Color - The color of the quad, each corner will have the same color
	 */
	static void BuildQuad(FProceduralMeshData &OutMeshData, const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &InNormal, const FVector &InTangent, const FColor &Color);

};
