// Fill out your copyright notice in the Description page of Project Settings.

#include "ResizePoint.h"

// Sets default values
AResizePoint::AResizePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	MeshComponent->SetRelativeScale3D(FVector(0.3, 0.3, 0.3));

	SetRootComponent(MeshComponent);
}