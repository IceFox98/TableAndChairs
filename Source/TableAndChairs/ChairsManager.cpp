// Fill out your copyright notice in the Description page of Project Settings.


#include "ChairsManager.h"
#include "Chair.h"

// Sets default values for this component's properties
UChairsManager::UChairsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ChairSeatSize = FVector(60.f, 60.f, 10.f);
	ChairBackSize = FVector(60.f, 15.f, 80.f);
	ChairLegSize = FVector(15.f, 15.f, 70.f);

	ChairsOnAxis.Add(EAxes::X);
	ChairsOnAxis.Add(EAxes::Y);

	DistanceFromMesh = .25f;
	ChairOffset = 15.f;
}


// Called when the game starts
void UChairsManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UChairsManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UChairsManager::Initialize(const FVector &MeshLegSize, AActor* ParentActor)
{
	Parent = ParentActor;
	ParentLegSize = MeshLegSize;

	ChairWidthWithOffset = ChairSeatSize.X + (ChairOffset * 2);

	const float ChairLegSeatHeight = ChairSeatSize.Z + ChairLegSize.Z;
	ChairOffsetZ = (ChairSeatSize.Z * .5f) + (ParentLegSize.Z - ChairLegSeatHeight);
}

void UChairsManager::UpdateChairs(const FVector &MeshSize)
{
	if (!Parent || !Parent->GetRootComponent())
	{
		UE_LOG(LogTemp, Error, TEXT("The Parent Actor or its Root is null. Unable to generate/update chairs."));
		return;
	}

	const FVector MeshExtent = MeshSize * .5f;

	const float TableSizeAvailableX = MeshSize.X - (ParentLegSize.X * 2);
	const float TableSizeAvailableY = MeshSize.Y - (ParentLegSize.Y * 2);

	const int32 ChairsPerSideX = FMath::FloorToInt(TableSizeAvailableX / ChairWidthWithOffset);
	const int32 ChairsPerSideY = FMath::FloorToInt(TableSizeAvailableY / ChairWidthWithOffset);

	//VERTICAL SIDES - Top and Bottom - Need to flip X-Axis
	float TotalChairLength = GetTotalChairLength(ChairsPerSideY, TableSizeAvailableY);

	FVector StartSpawnPoint = FVector(0, +MeshExtent.Y - ParentLegSize.Y, -MeshExtent.Z); // Starting from right-center of table
	FVector SpawnOffset = FVector(MeshExtent.X + DistanceFromMesh, (-TotalChairLength * .5f), -ChairOffsetZ);

	CalculateChairsOfAxis(StartSpawnPoint, SpawnOffset, ChairsPerSideY, TotalChairLength, EAxes::X);

	//HORIZONTAL SIDES - Right and Left - Need to flip Y-Axis
	TotalChairLength = GetTotalChairLength(ChairsPerSideX, TableSizeAvailableX);

	StartSpawnPoint = FVector(+MeshExtent.X - ParentLegSize.X, 0, -MeshExtent.Z); // Starting from top-center of table
	SpawnOffset = FVector((-TotalChairLength * .5f), MeshExtent.Y + DistanceFromMesh, -ChairOffsetZ);

	CalculateChairsOfAxis(StartSpawnPoint, SpawnOffset, ChairsPerSideX, TotalChairLength, EAxes::Y);
}

float UChairsManager::GetTotalChairLength(const int ChairsPerSide, const float TableSideLength) const
{
	if (ChairsPerSide == 0) //Can't divide by 0
	{
		return 0;
	}

	float TotalChairsLength = ChairsPerSide * ChairWidthWithOffset;
	float AdditionalChairOffset = (TableSideLength - TotalChairsLength) / ChairsPerSide;

	return ChairWidthWithOffset + AdditionalChairOffset;
}

void UChairsManager::CalculateChairsOfAxis(FVector StartSpawnPoint, FVector SpawnOffset, const int ChairsPerSide, const float TotalChairLength, const EAxes FlipAxis)
{
	//Get the rotation of the table
	float InitialRotation = Parent->GetActorRotation().Yaw;

	if (FlipAxis == EAxes::X)
	{
		//Initial rotation for vertical sides
		InitialRotation -= 90.f;
	}

	TArray<UChair*> &CurrChairs = ChairsOnAxis[FlipAxis].Chairs;//Get reference of array, in order to keep TMap updated

	const int32 AvailableChairsCount = GetAvailableChairsCount(FlipAxis);

	if ((AvailableChairsCount * .5f) < ChairsPerSide) //Adding
	{
		const int32 ChairsToAdd = ChairsPerSide - (AvailableChairsCount * .5f);
		for (int32 k = 0; k < ChairsToAdd; k++)
		{
			SpawnChair(FlipAxis);
			SpawnChair(FlipAxis);
		}
	}
	else if (AvailableChairsCount * .5f > ChairsPerSide && AvailableChairsCount >= 2) //Deleting (Hide)
	{
		SetPooledChair(FlipAxis);
		SetPooledChair(FlipAxis);
	}

	int32 i = 0;
	while (i < CurrChairs.Num() - 1) //Updating
	{
		UChair* ChairToUpdate = CurrChairs[i++];
		if (!ChairToUpdate->IsVisible()) { continue; }

		FixChairTransform(*ChairToUpdate, StartSpawnPoint, SpawnOffset, InitialRotation, EAxes::None);

		ChairToUpdate = CurrChairs[i++];
		if (!ChairToUpdate->IsVisible()) { continue; }

		FixChairTransform(*ChairToUpdate, StartSpawnPoint, SpawnOffset, InitialRotation + 180.f, FlipAxis);

		//Updating spawn point position
		//If flip Axis is Y, then you're updating chairs on X Axis
		if (FlipAxis == EAxes::Y)
		{
			StartSpawnPoint.X -= TotalChairLength;
		}
		else if (FlipAxis == EAxes::X)
		{
			StartSpawnPoint.Y -= TotalChairLength;
		}
	}
}

void UChairsManager::SpawnChair(const EAxes FlipAxis)
{
	if (!ChairClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ChairClass is NULL. Unable to spawn the chair!"));
		return;
	}

	UChair* PooledChair = GetPooledChair(FlipAxis);

	if (PooledChair)
	{
		PooledChair->SetVisibility(true);
		return;
	}

	//If there're any visible chairs, then spawn one.
	UChair* ChairSpawned = NewObject<UChair>(this, ChairClass);

	if (!ChairSpawned)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn the chair!"));
		return;
	}

	//Build spawned chair and add to Chairs array
	ChairSpawned->SetupAttachment(Parent->GetRootComponent());
	ChairSpawned->RegisterComponent();
	ChairSpawned->SetVisibility(true);

	ChairSpawned->BuildMesh(ChairSeatSize, ChairBackSize, ChairLegSize);
	ChairsOnAxis[FlipAxis].Chairs.Add(ChairSpawned);

}

#pragma region Pooling

UChair* UChairsManager::GetPooledChair(const EAxes FlipAxis) const
{
	TArray<UChair*> CurrChairs = ChairsOnAxis[FlipAxis].Chairs;

	for (int32 i = 0; i < CurrChairs.Num(); i++)
	{
		if (!CurrChairs[i]->IsVisible())
		{
			return CurrChairs[i];
		}
	}

	return nullptr;
}

void UChairsManager::SetPooledChair(const EAxes FlipAxis)
{
	TArray<UChair*> &CurrChairs = ChairsOnAxis[FlipAxis].Chairs;

	for (int32 i = 0; i < CurrChairs.Num(); i++)
	{
		if (CurrChairs[i]->IsVisible())
		{
			CurrChairs[i]->SetVisibility(false);
			return;
		}
	}
}

int32 UChairsManager::GetAvailableChairsCount(const EAxes FlipAxis) const
{
	TArray<UChair*> CurrChairs = ChairsOnAxis[FlipAxis].Chairs;
	int32 AvailableChairsCount = 0;

	for (int32 k = 0; k < CurrChairs.Num(); k++)
	{
		if (CurrChairs[k]->IsVisible())
		{
			AvailableChairsCount++;
		}
	}

	return AvailableChairsCount;
}

#pragma endregion Pooling

void UChairsManager::FixChairTransform(UChair &Chair, FVector StartSpawnPoint, FVector SpawnOffset, float Yaw, const EAxes FlipAxis) const
{
	if (FlipAxis == EAxes::Y)
	{
		SpawnOffset.Y *= -1;
	}
	if (FlipAxis == EAxes::X)
	{
		SpawnOffset.X *= -1;
	}

	//Set Transform
	const FVector NewLocation = StartSpawnPoint + SpawnOffset;
	Chair.SetRelativeLocation(NewLocation);
	Chair.SetRelativeRotation(FRotator(0, Yaw, 0));
}