// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"

AChair::AChair()
{
	VertexCount = (6 * 4) * 6; //(6 faces with 4 vertices each) = 1 cube/piece. Chair has 6 pieces
	Vertices.AddUninitialized(VertexCount);
	Normals.AddUninitialized(VertexCount);

	TrianglesCount = (6 * 2 * 3) * 6; //6 faces per cube, 2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);
}

void AChair::BeginPlay()
{
	Super::BeginPlay();

	BuildMesh();
}

void AChair::BuildMesh()
{
	Super::BuildMesh();

	BuildCube(SeatSize, FVector::ZeroVector); //Seat
	BuildCube(BackSize, FVector(0, (SeatSize.Y - BackSize.Y) * .5f, (SeatSize.Z + BackSize.Z) * .5f)); //Back

	BuildCube(LegSize, FVector((-SeatSize.X + LegSize.X) * .5f, (-SeatSize.Y + LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f)); //Leg 1
	BuildCube(LegSize, FVector((-SeatSize.X + LegSize.X) * .5f, (SeatSize.Y - LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f)); //Leg 2
	BuildCube(LegSize, FVector((SeatSize.X - LegSize.X) * .5f, (-SeatSize.Y + LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f)); //Leg 3
	BuildCube(LegSize, FVector((SeatSize.X - LegSize.X) * .5f, (SeatSize.Y - LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f)); //Leg 4

	GenerateMesh(TArray<FColor>());
}
