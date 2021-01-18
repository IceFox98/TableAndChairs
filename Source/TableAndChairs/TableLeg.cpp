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

	TrianglesCount = (6 * 2 * 3); //6 faces per cube, 2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);

	Width = 30;
	Length = 30;
	Height = 100;

	BuildMesh();
}

void ATableLeg::BuildMesh()
{
	Super::BuildMesh();

	BuildCube(FVector(0, 0, 0), FVector(0, 1, 0), FVector(0, 1, 1), FVector(0, 0, 1), FVector(1, 1, 0), FVector(1, 0, 0), FVector(1, 0, 1), FVector(1, 1, 1));

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Vertices[i] *= FVector(Width, Length, Height);
	}

}
