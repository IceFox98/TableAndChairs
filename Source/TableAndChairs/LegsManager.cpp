// Fill out your copyright notice in the Description page of Project Settings.


#include "LegsManager.h"
#include "Leg.h"

// Sets default values for this component's properties
ULegsManager::ULegsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	LegSize = FVector(30.f, 30.f, 100.f);
}

// Called when the game starts
void ULegsManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ULegsManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULegsManager::BuildLegs()
{
	const int32 LegsCount = 4;

	//Spawn legs
	for (int i = 0; i < LegsCount; i++)
	{
		ULeg* LegSpawned = NewObject<ULeg>(this, ULeg::StaticClass());

		if (LegSpawned)
		{
			LegSpawned->SetupAttachment(GetOwner()->GetRootComponent());
			LegSpawned->RegisterComponent();
			LegSpawned->BuildMesh(FVector::ZeroVector, LegSize);

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
		ULeg* Leg = Legs[i];

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