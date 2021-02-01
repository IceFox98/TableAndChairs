// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizePointManager.h"
#include "ResizePoint.h"

// Sets default values for this component's properties
UResizePointManager::UResizePointManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UResizePointManager::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//ATable* Table = Cast<ATable>(GetOwner());

}


// Called every frame
void UResizePointManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UResizePointManager::InitializePoints(const FVector &Center, const FVector &Extent)
{

	//Test = NewObject<UResizePoint>(this);
	//Test->RegisterComponent();
	//Test->SetupAttachment(GetOwner()->GetRootComponent());


	const TArray<FVector> Positions = {
		FVector(Center.X - Extent.X, Center.Y - Extent.Y, Center.Z + Extent.Z),
		FVector(Center.X + Extent.X, Center.Y - Extent.Y, Center.Z + Extent.Z),
		FVector(Center.X - Extent.X, Center.Y + Extent.Y, Center.Z + Extent.Z),
		FVector(Center.X + Extent.X, Center.Y + Extent.Y, Center.Z + Extent.Z),
	};

	for (int32 i = 0; i < Positions.Num(); i++)
	{
		FName Name = *FString::Printf(TEXT("ResizePoint_%i"), i);
		UResizePoint* ResizePoint = NewObject<UResizePoint>(this, UResizePoint::StaticClass(), Name);

		ResizePoint->RegisterComponent();
		ResizePoint->SetupAttachment(GetOwner()->GetRootComponent());
		ResizePoint->SetRelativeLocation(Positions[i]);

		ResizePoints.Add(ResizePoint);
	}
}

