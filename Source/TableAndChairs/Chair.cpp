// Fill out your copyright notice in the Description page of Project Settings.


#include "Chair.h"

// Sets default values for this component's properties
UChair::UChair(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UChair::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UChair::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UChair::BuildMesh(const FVector &SeatSize, const FVector &BackSize, const FVector &LegSize)
{
	BuildCube(SeatSize, FVector::ZeroVector, FColor::Black); //Seat
	BuildCube(BackSize, FVector(0, (SeatSize.Y - BackSize.Y) * .5f, (SeatSize.Z + BackSize.Z) * .5f), FColor::Black); //Back

	BuildCube(LegSize, FVector((-SeatSize.X + LegSize.X) * .5f, (-SeatSize.Y + LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 1
	BuildCube(LegSize, FVector((-SeatSize.X + LegSize.X) * .5f, (SeatSize.Y - LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 2
	BuildCube(LegSize, FVector((SeatSize.X - LegSize.X) * .5f, (-SeatSize.Y + LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 3
	BuildCube(LegSize, FVector((SeatSize.X - LegSize.X) * .5f, (SeatSize.Y - LegSize.Y) * .5f, (-SeatSize.Z - LegSize.Z) * .5f), FColor::Black); //Leg 4

	GenerateMesh();
}
