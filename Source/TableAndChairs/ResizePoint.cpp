// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizePoint.h"

// Sets default values
AResizePoint::AResizePoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	MeshComponent->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	//MeshComponent->SetCollisionProfileName();

	SetRootComponent(MeshComponent);

	//MeshComponent->SetupAttachment(RootComponent);



	//CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	//CollisionComponent->InitSphereRadius(10.0f);
	//CollisionComponent->SetupAttachment(MeshComponent);
}

// Called when the game starts or when spawned
void AResizePoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResizePoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

