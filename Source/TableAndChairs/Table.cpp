// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
#include "ResizePointManager.h"
#include "LegsManager.h"
#include "ChairsManager.h"
#include "DynamicMeshLibrary.h"

// Sets default values
ATable::ATable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>("DynamicMesh");
	SetRootComponent(ProceduralMeshComponent);

	ResizePointManager = CreateDefaultSubobject<UResizePointManager>("ResizePointManager");
	LegsManager = CreateDefaultSubobject<ULegsManager>("LegsManager");
	ChairsManager = CreateDefaultSubobject<UChairsManager>("ChaisManager");

	Size = FVector(400.f, 400.f, 20.f);
	CurrentSize = Size;

	MaxSize = FVector(1000.f, 1000.f, 20.f);
	MinSize = FVector(200.f, 200.f, 20.f);
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	//Create table mesh
	BuildMesh(GetActorLocation(), Size, false);

	//Calculate table location
	const FVector LegSize = LegsManager->GetLegSize();
	const FVector PositionOffset = FVector(0, 0, LegSize.Z + (Size.Z * .5f));
	SetActorLocation(GetActorLocation() + PositionOffset);

	USceneComponent* const Root = GetRootComponent();

	//Generate Resize Points
	const FVector Extent = Size * .5f;
	ResizePointManager->InitializePoints(Extent, Root);

	//Create legs
	LegsManager->BuildLegs(Root);
	LegsManager->UpdateLegsPosition(Extent);

	//Create Chairs
	ChairsManager->Initialize(LegSize, Root);
	ChairsManager->UpdateChairs(Size);
}

void ATable::BuildMesh(const FVector &Center, const FVector &MeshSize, const bool Update)
{
	FProceduralMeshData Data;
	UDynamicMeshLibrary::BuildCube(Data, MeshSize, FVector::ZeroVector, FColor::Red);

	if (Update)
	{
		ProceduralMeshComponent->UpdateMeshSection(0, Data.Vertices, Data.Normals, Data.UVs, Data.VertexColors, Data.Tangents);
	}
	else
	{
		ProceduralMeshComponent->CreateMeshSection(0, Data.Vertices, Data.Triangles, Data.Normals, Data.UVs, Data.VertexColors, Data.Tangents, true);
	}

	SetActorLocation(Center);
}

FVector ATable::ResizeMesh(const FVector &Direction, const FVector &NewExtent)
{
	const FVector DeltaSize = NewExtent.GetAbs() - (CurrentSize * .5f);

	if (DeltaSize != FVector::ZeroVector)
	{
		//Move the Actor by half of the movement
		FVector DeltaPosition = (DeltaSize * Direction) * .5f;
		DeltaPosition.Z = 0;
		const FVector NewCenter = GetActorLocation() + DeltaPosition;

		//Get new size
		FVector NewSize = CurrentSize + DeltaSize;
		NewSize.Z = Size.Z;

		BuildMesh(NewCenter, NewSize, true);

		CurrentSize = NewSize;
		LegsManager->UpdateLegsPosition(NewSize * .5f);
		ChairsManager->UpdateChairs(NewSize);
	}

	return DeltaSize;
}

FVector ATable::ClampSize(const FVector &Direction, const FVector &SizeToCheck)
{
	const FVector ActorLocation = GetActorLocation();

	FVector ReturnSize = SizeToCheck;

	//Just because when we resize the mesh, the pivot is always in the center
	const FVector MaxSizeHalf = MaxSize * .5f;
	const FVector MinSizeHalf = MinSize * .5f;

	float DiffX = FMath::Abs(SizeToCheck.X - ActorLocation.X);
	float DiffY = FMath::Abs(SizeToCheck.Y - ActorLocation.Y);

	//When you exceed (with the mouse) the min size, re-set the abs difference so that it clamps automatically
	if (FMath::Sign(SizeToCheck.X - ActorLocation.X) != FMath::Sign(Direction.X))
	{
		DiffX = MinSizeHalf.X - 1.f; //Just for clamp
	}
	if (FMath::Sign(SizeToCheck.Y - ActorLocation.Y) != FMath::Sign(Direction.Y))
	{
		DiffY = MinSizeHalf.Y - 1.f;
	}

	//Clamping
	if (DiffX > MaxSizeHalf.X || DiffX < MinSizeHalf.X)
	{
		ReturnSize.X = (FMath::Clamp(DiffX, MinSizeHalf.X, MaxSizeHalf.X) * Direction.X) + ActorLocation.X;
	}
	if (DiffY > MaxSizeHalf.Y || DiffY < MinSizeHalf.Y)
	{
		ReturnSize.Y = (FMath::Clamp(DiffY, MinSizeHalf.Y, MaxSizeHalf.Y) * Direction.Y) + ActorLocation.Y;
	}

	return ReturnSize;
}

FVector ATable::GetMeshSize()
{
	return Size;
}
