// Fill out your copyright notice in the Description page of Project Settings.


#include "Leg.h"

// Sets default values for this component's properties
ULeg::ULeg(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void ULeg::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ULeg::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULeg::BuildMesh(const FVector &Center, const FVector &Size)
{
	BuildCube(Size, FVector::ZeroVector, FColor::Black);
	GenerateMesh();

	SetRelativeLocation(Center);
}

