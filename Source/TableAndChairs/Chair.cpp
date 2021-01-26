// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"

AChair::AChair()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AChair::BeginPlay()
{
	Super::BeginPlay();

	if (GameMode)
	{
		SeatSize = GameMode->ChairSeatSize;
		BackSize = GameMode->ChairBackSize;
		LegSize = GameMode->ChairLegSize;
	}

	BuildMesh();
}

void AChair::BuildMesh()
{
	Super::BuildMesh();

	BuildCube(SeatSize, FVector::ZeroVector, FColor::Black); //Seat
	BuildCube(BackSize, FVector(0, (SeatSize.Y - BackSize.Y) * .5f, (SeatSize.Z + BackSize.Z) * .5f), FColor::Black); //Back

	BuildCube(LegSize, FVector((-SeatSize.X + LegSize.X) * .5f, (-SeatSize.Y + LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 1
	BuildCube(LegSize, FVector((-SeatSize.X + LegSize.X) * .5f, (SeatSize.Y - LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 2
	BuildCube(LegSize, FVector((SeatSize.X - LegSize.X) * .5f, (-SeatSize.Y + LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 3
	BuildCube(LegSize, FVector((SeatSize.X - LegSize.X) * .5f, (SeatSize.Y - LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 4

	GenerateMesh();
}
