// Fill out your copyright notice in the Description page of Project Settings.

#include "TableLeg.h"

ATableLeg::ATableLeg()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATableLeg::BeginPlay()
{
	Super::BeginPlay();

	if (GameMode)
	{
		Size = GameMode->TableLegSize;
	}

	BuildMesh();
}

void ATableLeg::BuildMesh()
{
	Super::BuildMesh();

	BuildCube(Size, FVector::ZeroVector, FColor::Black);

	GenerateMesh();
}
