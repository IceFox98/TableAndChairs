// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizePoint.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UResizePoint::UResizePoint()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	MeshComponent->SetStaticMesh(SphereMeshAsset.Object);
	MeshComponent->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));
	//MeshComponent->SetCollisionProfileName();



	MeshComponent->SetupAttachment(GetAttachmentRoot());



	//CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	//CollisionComponent->InitSphereRadius(10.0f);
	//CollisionComponent->SetupAttachment(MeshComponent);


}


// Called when the game starts
void UResizePoint::BeginPlay()
{
	Super::BeginPlay();


}


// Called every frame
void UResizePoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

