// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizePoint.h"
#include "ResizePointManager.h"

// Sets default values for this component's properties
UResizePoint::UResizePoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UResizePoint::BeginPlay()
{
	Super::BeginPlay();

	ResizePointManager = Cast<UResizePointManager>(GetOwner()->GetComponentByClass(UResizePointManager::StaticClass()));
}

void UResizePoint::CheckPosition(const FVector &NewPosition)
{
	bool IsValid = true; //Add any checks here

	if (ResizePointManager)
	{
		ResizePointManager->OnResizePointMovedDelegate.ExecuteIfBound(IsValid, this, NewPosition);
	}
}