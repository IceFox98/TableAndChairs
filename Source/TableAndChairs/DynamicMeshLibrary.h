// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralMeshComponent.h"
#include "DynamicMeshLibrary.generated.h"


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
	 * Adds the Vertices, Triangles and Normals necessary to create the mesh
	 * @param MeshSize - The size of the Cube
	 * @param Position - Where the mesh will be positioned. NOTE: Its starting pivot will be always (0, 0, 0)
	 */
	static void BuildCube(FProceduralMeshData &OutMeshData, const FVector &MeshSize, const FVector &Position, const FColor &Color);

private:

	static void BuildQuad(FProceduralMeshData &OutMeshData, const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &InNormal, const FVector &InTangent, const FColor &Color);

};
