// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizePoint.h"
#include "ResizePointManager.h"

// Sets default values for this component's properties
UResizePoint::UResizePoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	SetStaticMesh(SphereMeshAsset.Object);
	SetRelativeScale3D(FVector(0.3, 0.3, 0.3));

}


// Called when the game starts
void UResizePoint::BeginPlay()
{
	Super::BeginPlay();

	ResizePointManager = Cast<UResizePointManager>(GetOwner()->GetComponentByClass(UResizePointManager::StaticClass()));
}


// Called every frame
void UResizePoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UResizePoint::SetPosition(const FVector &NewLocation)
{
	SetRelativeLocation(NewLocation);

	if (ResizePointManager)
	{
		ResizePointManager->OnResizePointMovedDelegate.ExecuteIfBound(NewLocation);
	}
}

