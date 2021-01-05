// Fill out your copyright notice in the Description page of Project Settings.


#include "TableLeg.h"

ATableLeg::ATableLeg()
{

}

void ATableLeg::BeginPlay()
{
	Super::BeginPlay();

	VertexCount = (6 * 4); //6 faces with 4 vertices each
	Vertices.AddUninitialized(VertexCount);

	TrianglesCount = (6 * 2 * 3); //2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);

	Width = 10;
	Length = 10;
	Height = 100;

	BuildMesh();
}

void ATableLeg::BuildMesh()
{
	Super::BuildMesh();

	FVector V1 = FVector(0, 0, -1); //Front - Bottom Left
	FVector V2 = FVector(1, 0, -1); //Front - Bottom Right
	FVector V3 = FVector(1, 0, 0); //Front - Top Right
	FVector V4 = FVector(0, 0, 0); //Front - Top Left
	FVector V5 = FVector(1, -1, -1); //Back - Bottom Left
	FVector V6 = FVector(0, -1, -1); //Back - Bottom Right
	FVector V7 = FVector(0, -1, 0); //Back - Top Right
	FVector V8 = FVector(1, -1, 0); //Back - Top Left

	BuildQuad(V1, V2, V3, V4); //Front Face
	BuildQuad(V2, V5, V8, V3); //Right Face
	BuildQuad(V6, V1, V4, V7); //Left Face
	BuildQuad(V5, V6, V7, V8); //Back Face
	BuildQuad(V4, V3, V8, V7); //Top Face
	BuildQuad(V6, V5, V2, V1); //Bottom Face

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Vertices[i] *= FVector(Width, Length, Height);
	}

	//TArray<FLinearColor> VertexColors;

	//Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

}
