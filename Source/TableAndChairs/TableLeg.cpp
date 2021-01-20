// Fill out your copyright notice in the Description page of Project Settings.

#include "TableLeg.h"

ATableLeg::ATableLeg()
{
	VertexCount = (6 * 4); //6 faces with 4 vertices each
	Vertices.AddUninitialized(VertexCount);
	Normals.AddUninitialized(VertexCount);

	TrianglesCount = (6 * 2 * 3); //6 faces per cube, 2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);
}

void ATableLeg::BeginPlay()
{
	Super::BeginPlay();

	BuildMesh();
}

void ATableLeg::BuildMesh()
{
	Super::BuildMesh();

	BuildCube(Size, FVector::ZeroVector);

	GenerateMesh(TArray<FColor>());
}
