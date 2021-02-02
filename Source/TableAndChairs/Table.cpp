// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
#include "DynamicMeshComponent.h"
#include "ResizePointManager.h"

// Sets default values
ATable::ATable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>("DynamicMesh");
	SetRootComponent(DynamicMeshComponent);

	ResizePointManager = CreateDefaultSubobject<UResizePointManager>("ResizePointManager");

	Size = FVector(400.f, 400.f, 20.f);
	LegSize = FVector(30.f, 30.f, 100.f);

	MaxSize = FVector(1000.f, 1000.f, 20.f);
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	BuildMesh(FVector::ZeroVector, Size);

	const FVector PositionOffset = FVector(0, 0, LegSize.Z + (Size.Z * .5f));
	SetActorLocation(GetActorLocation() + PositionOffset);

	const FVector Extent = Size * .5f;
	ResizePointManager->InitializePoints(GetActorLocation(), Extent);

}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATable::BuildMesh(const FVector &Center, const FVector &Extent)
{
	DynamicMeshComponent->BuildCube(Extent, FVector::ZeroVector, FColor::Red);
	DynamicMeshComponent->GenerateMesh();

	SetActorLocation(Center);
}

bool ATable::ResizeMesh(const FVector &Center, const FVector &Extent)
{

	if (Extent.X > MaxSize.X || Extent.Y > MaxSize.Y)
	{
		return false;
	}

	DynamicMeshComponent->ResetBuffers();

	FVector NewPosition = Center;
	NewPosition.Z = 0;

	FVector NewSize = Extent;
	NewSize.Z = Size.Z;

	DynamicMeshComponent->BuildCube(NewSize, NewPosition, FColor::Red);
	DynamicMeshComponent->UpdateMesh();

	return true;
	//SetActorLocation(NewPosition);

}

