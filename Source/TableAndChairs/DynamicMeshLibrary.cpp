// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicMeshLibrary.h"

void UDynamicMeshLibrary::BuildQuad(FProceduralMeshData &OutMeshData, const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &InNormal, const FVector &InTangent, const FColor &Color)
{
	const int32 VertexCount = 4; //4 vertices for each quad
	const int32 TrianglesCount = 2 * 3; //2 triangles per quad, 3 vertices each

	int32 VertexIndex = OutMeshData.Vertices.Num();
	int32 TrianglesIndex = OutMeshData.Triangles.Num();

	OutMeshData.Vertices.AddUninitialized(VertexCount);
	OutMeshData.Triangles.AddUninitialized(TrianglesCount);
	OutMeshData.Normals.AddUninitialized(VertexCount);
	OutMeshData.VertexColors.AddUninitialized(VertexCount);
	OutMeshData.UVs.AddUninitialized(VertexCount);
	OutMeshData.Tangents.AddUninitialized(VertexCount);

	//OutMeshData.Vertices.iter

	const int32 Index1 = VertexIndex++;
	const int32 Index2 = VertexIndex++;
	const int32 Index3 = VertexIndex++;
	const int32 Index4 = VertexIndex++;

	if (VertexIndex > OutMeshData.Vertices.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("VertexIndex does not correspond to Vertices Count. Mesh could appear incomplete."));
		return;
	}

	OutMeshData.Vertices[Index1] = BottomLeft;
	OutMeshData.Vertices[Index2] = BottomRight;
	OutMeshData.Vertices[Index3] = TopRight;
	OutMeshData.Vertices[Index4] = TopLeft;

	OutMeshData.VertexColors[Index1] = Color;
	OutMeshData.VertexColors[Index2] = Color;
	OutMeshData.VertexColors[Index3] = Color;
	OutMeshData.VertexColors[Index4] = Color;

	//Unreal's UVs start from upper-left and proceeds clockwise
	OutMeshData.UVs[Index1] = FVector2D(0.f, 1.f);
	OutMeshData.UVs[Index2] = FVector2D(1.f, 1.f);
	OutMeshData.UVs[Index3] = FVector2D(1.f, 0.f);
	OutMeshData.UVs[Index4] = FVector2D(0.f, 0.f);

	OutMeshData.Triangles[TrianglesIndex++] = Index1;
	OutMeshData.Triangles[TrianglesIndex++] = Index2;
	OutMeshData.Triangles[TrianglesIndex++] = Index3;
	OutMeshData.Triangles[TrianglesIndex++] = Index1;
	OutMeshData.Triangles[TrianglesIndex++] = Index3;
	OutMeshData.Triangles[TrianglesIndex++] = Index4;

	// In a cube all vertices of a face have the same normals and tangents
	OutMeshData.Normals[Index1] = OutMeshData.Normals[Index2] = OutMeshData.Normals[Index3] = OutMeshData.Normals[Index4] = InNormal;
	OutMeshData.Tangents[Index1] = OutMeshData.Tangents[Index2] = OutMeshData.Tangents[Index3] = OutMeshData.Tangents[Index4] = FProcMeshTangent(InTangent, false);
}


void UDynamicMeshLibrary::BuildCube(FProceduralMeshData &OutMeshData, const FVector &MeshSize, const FVector &Position, const FColor &Color)
{
	// Divide in half the offset to get center of object
	const float OffsetX = MeshSize.X * .5f;
	const float OffsetY = MeshSize.Y * .5f;
	const float OffsetZ = MeshSize.Z * .5f;

	FVector V1 = FVector(-OffsetX, -OffsetY, -OffsetZ) + Position; //Front - Bottom Left
	FVector V2 = FVector(-OffsetX, OffsetY, -OffsetZ) + Position; //Front - Bottom Right
	FVector V3 = FVector(-OffsetX, OffsetY, OffsetZ) + Position; //Front - Top Right
	FVector V4 = FVector(-OffsetX, -OffsetY, OffsetZ) + Position; //Front - Top Left
	FVector V5 = FVector(OffsetX, OffsetY, -OffsetZ) + Position; //Back - Bottom Left
	FVector V6 = FVector(OffsetX, -OffsetY, -OffsetZ) + Position; //Back - Bottom Right
	FVector V7 = FVector(OffsetX, -OffsetY, OffsetZ) + Position; //Back - Top Right
	FVector V8 = FVector(OffsetX, OffsetY, OffsetZ) + Position; //Back - Top Left

	//Tangent is the right vector of the normal
	BuildQuad(OutMeshData, V1, V2, V3, V4, FVector(-1, 0, 0), FVector(0, -1, 0), Color); //Front face
	BuildQuad(OutMeshData, V6, V1, V4, V7, FVector(0, -1, 0), FVector(1, 0, 0), Color); //Left face
	BuildQuad(OutMeshData, V2, V5, V8, V3, FVector(0, 1, 0), FVector(-1, 0, 0), Color); //Right face
	BuildQuad(OutMeshData, V5, V6, V7, V8, FVector(1, 0, 0), FVector(0, 1, 0), Color); //Back face
	BuildQuad(OutMeshData, V4, V3, V8, V7, FVector(0, 0, 1), FVector(0, 1, 0), Color); //Top face
	BuildQuad(OutMeshData, V6, V5, V2, V1, FVector(0, 0, -1), FVector(0, -1, 0), Color); //Bottom face
}
