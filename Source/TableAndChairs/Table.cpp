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
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	BuildMesh(FVector::ZeroVector, FVector(400, 400, 20));

	ResizePointManager->InitializePoints(GetActorLocation(), FVector(200, 200, 10));
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

