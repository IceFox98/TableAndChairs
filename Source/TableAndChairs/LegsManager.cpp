// Fill out your copyright notice in the Description page of Project Settings.


#include "LegsManager.h"
#include "DynamicMeshLibrary.h"

// Sets default values for this component's properties
ULegsManager::ULegsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	LegSize = FVector(30.f, 30.f, 100.f);
}

void ULegsManager::BuildLegs(USceneComponent* ParentComp)
{
	if (!ParentComp)
	{
		UE_LOG(LogTemp, Error, TEXT("The Parent Component is null. Unable to generate/update legs."));
		return;
	}

	const int32 LegsCount = 4;

	FProceduralMeshData LegMeshData;
	UDynamicMeshLibrary::BuildCube(LegMeshData, LegSize, FVector::ZeroVector, FColor::Black);

	//Spawn legs
	for (int i = 0; i < LegsCount; i++)
	{
		UProceduralMeshComponent* LegSpawned = NewObject<UProceduralMeshComponent>(this, UProceduralMeshComponent::StaticClass());

		if (LegSpawned)
		{
			LegSpawned->SetupAttachment(ParentComp);
			LegSpawned->RegisterComponent();

			LegSpawned->CreateMeshSection(0, LegMeshData.Vertices, LegMeshData.Triangles, LegMeshData.Normals, LegMeshData.UVs, LegMeshData.VertexColors, LegMeshData.Tangents, true);

			Legs.Add(LegSpawned);
		}
	}
}

void ULegsManager::UpdateLegsPosition(const FVector &ParentExtent)
{
	const FVector HalfLegSize = LegSize * .5f;
	const float ZPosition = -ParentExtent.Z - HalfLegSize.Z;

	//Calculate new positions
	const TArray<FVector> LegsPositions = {
		FVector(-ParentExtent.X + HalfLegSize.X, -ParentExtent.Y + HalfLegSize.Y, ZPosition),
		FVector(-ParentExtent.X + HalfLegSize.X, +ParentExtent.Y - HalfLegSize.Y, ZPosition),
		FVector(+ParentExtent.X - HalfLegSize.X, -ParentExtent.Y + HalfLegSize.Y, ZPosition),
		FVector(+ParentExtent.X - HalfLegSize.X, +ParentExtent.Y - HalfLegSize.Y, ZPosition)
	};

	if (LegsPositions.Num() > Legs.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("There are too few legs. Checks the position count."));
		return;
	}

	//Update position
	for (int i = 0; i < LegsPositions.Num(); i++)
	{
		UProceduralMeshComponent* Leg = Legs[i];

		if (!Leg)
		{
			UE_LOG(LogTemp, Error, TEXT("Leg is NULL. Unable to move it."));
			return;
		}

		Leg->SetRelativeLocation(LegsPositions[i]);
	}
}

FVector ULegsManager::GetLegSize()
{
	return LegSize;
}