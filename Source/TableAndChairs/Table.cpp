// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
#include "DynamicMeshComponent.h"
#include "ResizePointManager.h"
#include "LegsManager.h"
#include "ChairsManager.h"

// Sets default values
ATable::ATable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>("DynamicMesh");
	SetRootComponent(DynamicMeshComponent);

	ResizePointManager = CreateDefaultSubobject<UResizePointManager>("ResizePointManager");
	LegsManager = CreateDefaultSubobject<ULegsManager>("LegsManager");
	ChairsManager = CreateDefaultSubobject<UChairsManager>("ChaisManager");

	Size = FVector(400.f, 400.f, 20.f);

	MaxSize = FVector(1000.f, 1000.f, 20.f);
	MinSize = FVector(200.f, 200.f, 20.f);
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	//Create table mesh
	BuildMesh(FVector::ZeroVector, Size, false);

	//Calculate table location
	const FVector LegSize = LegsManager->GetLegSize();
	const FVector PositionOffset = FVector(0, 0, LegSize.Z + (Size.Z * .5f));
	SetActorLocation(GetActorLocation() + PositionOffset);

	//Generate Resize Points
	const FVector Extent = Size * .5f;
	ResizePointManager->InitializePoints(GetActorLocation(), Extent);

	//Create legs
	LegsManager->BuildLegs();
	LegsManager->UpdateLegsPosition(Extent);

	//Create Chairs
	ChairsManager->Initialize(LegSize, this);
	ChairsManager->UpdateChairs(Size);
}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATable::BuildMesh(const FVector &Center, const FVector &MeshSize, const bool Update)
{
	DynamicMeshComponent->BuildCube(MeshSize, FVector::ZeroVector, FColor::Red);

	if (Update)
	{
		DynamicMeshComponent->UpdateMesh();
	}
	else
	{
		DynamicMeshComponent->GenerateMesh();
	}

	SetActorLocation(Center);
}

FVector ATable::ResizeMesh(const FVector &Direction, const FVector &DeltaSize)
{
	if (DeltaSize != FVector::ZeroVector)
	{
		//Move the Actor by half of the movement
		FVector DeltaPosition = (DeltaSize) * .5f;
		DeltaPosition.Z = 0;
		const FVector NewCenter = GetActorLocation() + DeltaPosition;

		//Get new size
		const FVector CurrentSize = DynamicMeshComponent->Bounds.GetBox().GetSize();
		FVector NewSize = (CurrentSize + (DeltaSize * Direction));
		NewSize.Z = Size.Z;

		DynamicMeshComponent->ResetBuffers();
		BuildMesh(NewCenter, NewSize, true);

		LegsManager->UpdateLegsPosition(NewSize * .5f);
		ChairsManager->UpdateChairs(NewSize);

	}

	//Return the center
	return GetActorLocation();
}

FVector ATable::ClampSize(const FVector &Direction, const FVector &SizeToCheck)
{
	////const FVector CurrentSize = DynamicMeshComponent->Bounds.GetBox().GetSize();

	//const FVector ActorLocation = GetActorLocation();

	////float DirectionX = FMath::Sign(SizeToCheck.X - ActorLocation.X);
	////float DirectionY = FMath::Sign(SizeToCheck.Y - ActorLocation.Y);

	//FVector AbsSize = SizeToCheck;

	////Just because when we resize the mesh, the pivot is always in the center
	//const FVector MaxSizeHalf = MaxSize * .5f;
	//const FVector MinSizeHalf = MinSize * .5f;

	////UE_LOG(LogTemp, Warning, TEXT("AbsSize: %s"), *AbsSize.ToString());

	//float DiffX = FMath::Abs(AbsSize.X - ActorLocation.X);
	//float DiffY = FMath::Abs(AbsSize.Y - ActorLocation.Y);


	////FMath::Sign(SizeToCheck.X - ActorLocation.X) == FMath::Sign(Direction.X) || 

	//if (DiffX >= MaxSizeHalf.X || DiffX <= MinSizeHalf.X)
	//{
	//	AbsSize.X = (FMath::Clamp(DiffX, MinSizeHalf.X, MaxSizeHalf.X) + GetActorLocation().X) * Direction.X;
	//}

	//UE_LOG(LogTemp, Warning, TEXT("ActorLocationX: %f - DiffY: %f"), ActorLocation.X, DiffY);
	////FMath::Sign(SizeToCheck.Y - ActorLocation.Y) == FMath::Sign(Direction.Y) || 

	//if (DiffY >= MaxSizeHalf.Y || DiffY <= MinSizeHalf.Y)
	//{
	//	AbsSize.Y = (FMath::Clamp(DiffY, MinSizeHalf.Y, MaxSizeHalf.Y) + GetActorLocation().Y) * Direction.Y;
	//}

	//return AbsSize;

	return SizeToCheck;
}
