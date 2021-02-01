// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralMesh.h"
#include "DynamicMeshComponent.h"

// Sets default values
AProceduralMesh::AProceduralMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DynamicMeshComponent = CreateDefaultSubobject<UDynamicMeshComponent>("DynamicMesh");
	SetRootComponent(DynamicMeshComponent);

}

// Called when the game starts or when spawned
void AProceduralMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProceduralMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

