// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"

AChair::AChair()
{
	VertexCount = (6 * 4) * 6; //(6 faces with 4 vertices each) = 1 cube/piece. Chair has 6 pieces
	Vertices.AddUninitialized(VertexCount);

	TrianglesCount = (6 * 2 * 3) * 6; //6 faces per cube, 2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);

	Width = 3;
	Length = 3;
	Height = 10;

}

void AChair::BeginPlay()
{
	Super::BeginPlay();

	BuildMesh();
}

void AChair::BuildMesh()
{
	Super::BuildMesh();

	BuildCube(FVector(-10, -10, -1), FVector(-10, 10, -1), FVector(-10, 10, 0), FVector(-10, -10, 0), FVector(10, 10, -1), FVector(10, -10, -1), FVector(10, -10, 0), FVector(10, 10, 0)); //Seat
	BuildCube(FVector(-10, 7, 0), FVector(-10, 10, 0), FVector(-10, 10, 8), FVector(-10, 7, 8), FVector(10, 10, 0), FVector(10, 7, 0), FVector(10, 7, 8), FVector(10, 10, 8)); //Back
	BuildCube(FVector(-10, -10, -8), FVector(-10, -7, -8), FVector(-10, -7, -1), FVector(-10, -10, -1), FVector(-7, -7, -8), FVector(-7, -10, -8), FVector(-7, -10, -1), FVector(-7, -7, -1)); //Leg 1
	BuildCube(FVector(-10, 7, -8), FVector(-10, 10, -8), FVector(-10, 10, -1), FVector(-10, 7, -1), FVector(-7, 10, -8), FVector(-7, 7, -8), FVector(-7, 7, -1), FVector(-7, 10, -1)); //Leg 2
	BuildCube(FVector(7, -10, -8), FVector(7, -7, -8), FVector(7, -7, -1), FVector(7, -10, -1), FVector(10, -7, -8), FVector(10, -10, -8), FVector(10, -10, -1), FVector(10, -7, -1)); //Leg 3
	BuildCube(FVector(7, 7, -8), FVector(7, 10, -8), FVector(7, 10, -1), FVector(7, 7, -1), FVector(10, 10, -8), FVector(10, 7, -8), FVector(10, 7, -1), FVector(10, 10, -1)); //Leg 4

	FVector Scale = FVector(Width, Length, Height);

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Vertices[i] *= Scale;
	}

	GenerateMesh(TArray<FLinearColor>());

}
