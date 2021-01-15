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
	//Nothing
}

void ADynamicMesh::GenerateMesh(TArray<FLinearColor> VertexColors)
{
	Mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
}

void ADynamicMesh::BuildQuad(const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft)
{
	const int32 Index1 = VertexIndex++;
	const int32 Index2 = VertexIndex++;
	const int32 Index3 = VertexIndex++;
	const int32 Index4 = VertexIndex++;

	Vertices[Index1] = BottomLeft;
	Vertices[Index2] = BottomRight;
	Vertices[Index3] = TopRight;
	Vertices[Index4] = TopLeft;

	Triangles[TrianglesIndex++] = Index1;
	Triangles[TrianglesIndex++] = Index2;
	Triangles[TrianglesIndex++] = Index3;
	Triangles[TrianglesIndex++] = Index1;
	Triangles[TrianglesIndex++] = Index3;
	Triangles[TrianglesIndex++] = Index4;
}

void ADynamicMesh::BuildCube(const FVector &FrontBottomLeft, const FVector &FrontBottomRight, const FVector &FrontTopRight, const FVector &FrontTopLeft, const FVector &BackBottomLeft, const FVector &BackBottomRight, const FVector &BackTopRight, const FVector &BackTopLeft)
{
	BuildQuad(FrontBottomLeft, FrontBottomRight, FrontTopRight, FrontTopLeft); //Front Face
	BuildQuad(FrontBottomRight, BackBottomLeft, BackTopLeft, FrontTopRight); //Right Face
	BuildQuad(BackBottomRight, FrontBottomLeft, FrontTopLeft, BackTopRight); //Left Face
	BuildQuad(BackBottomLeft, BackBottomRight, BackTopRight, BackTopLeft); //Back Face
	BuildQuad(FrontTopLeft, FrontTopRight, BackTopLeft, BackTopRight); //Top Face
	BuildQuad(BackBottomRight, BackBottomLeft, FrontBottomRight, FrontBottomLeft); //Bottom Face
}
