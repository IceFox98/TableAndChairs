// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicMesh.h"

// Sets default values
ADynamicMesh::ADynamicMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADynamicMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADynamicMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADynamicMesh::BuildMesh()
{

}

void ADynamicMesh::GenerateMesh(TArray<FLinearColor> VertexColors)
{
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

void ADynamicMesh::BuildQuad(const FVector bottomLeft, const FVector bottomRight, const FVector topRight, const FVector topLeft)
{
	const int32 Index1 = VertexIndex++;
	const int32 Index2 = VertexIndex++;
	const int32 Index3 = VertexIndex++;
	const int32 Index4 = VertexIndex++;

	Vertices[Index1] = bottomLeft;
	Vertices[Index2] = bottomRight;
	Vertices[Index3] = topRight;
	Vertices[Index4] = topLeft;

	Triangles[TrianglesIndex++] = Index1;
	Triangles[TrianglesIndex++] = Index2;
	Triangles[TrianglesIndex++] = Index3;
	Triangles[TrianglesIndex++] = Index1;
	Triangles[TrianglesIndex++] = Index3;
	Triangles[TrianglesIndex++] = Index4;
}
