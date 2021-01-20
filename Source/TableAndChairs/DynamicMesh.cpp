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

	VertexIndex = 0;
	TrianglesIndex = 0;
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

void ADynamicMesh::GenerateMesh(TArray<FColor> InVertexColors)
{
	VertexColors = InVertexColors;

	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);

	if (Material)
	{
		Mesh->SetMaterial(0, Material);
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Material selected!"));
	}
}

void ADynamicMesh::UpdateMesh()
{
	Mesh->UpdateMeshSection(0, Vertices, Normals, TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>());
}

void ADynamicMesh::BuildQuad(const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &InNormals)
{
	const int32 Index1 = VertexIndex++;
	const int32 Index2 = VertexIndex++;
	const int32 Index3 = VertexIndex++;
	const int32 Index4 = VertexIndex++;

	if (VertexIndex > Vertices.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("VertexIndex does not correspond to Vertices Count. Mesh could appear incomplete."));
		return;
	}

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

	// In a cube all vertices of a face have the same normals
	Normals[Index1] = Normals[Index2] = Normals[Index3] = Normals[Index4] = InNormals;
}

void ADynamicMesh::BuildCube(FVector MeshSize, FVector Position)
{
	// Divide in half the offset to get center of object
	const float OffsetX = MeshSize.X / 2.0f;
	const float OffsetY = MeshSize.Y / 2.0f;
	const float OffsetZ = MeshSize.Z / 2.0f;

	FVector V1 = FVector(-OffsetX, -OffsetY, -OffsetZ) + Position; //Front - Bottom Left
	FVector V2 = FVector(-OffsetX, OffsetY, -OffsetZ) + Position; //Front - Bottom Right
	FVector V3 = FVector(-OffsetX, OffsetY, OffsetZ) + Position; //Front - Top Right
	FVector V4 = FVector(-OffsetX, -OffsetY, OffsetZ) + Position; //Front - Top Left
	FVector V5 = FVector(OffsetX, OffsetY, -OffsetZ) + Position; //Back - Bottom Left
	FVector V6 = FVector(OffsetX, -OffsetY, -OffsetZ) + Position; //Back - Bottom Right
	FVector V7 = FVector(OffsetX, -OffsetY, OffsetZ) + Position; //Back - Top Right
	FVector V8 = FVector(OffsetX, OffsetY, OffsetZ) + Position; //Back - Top Left

	BuildQuad(V1, V2, V3, V4, FVector(-1, 0, 0)); //Front face
	BuildQuad(V6, V1, V4, V7, FVector(0, -1, 0)); //Left face
	BuildQuad(V2, V5, V8, V3, FVector(0, 1, 0)); //Right face
	BuildQuad(V5, V6, V7, V8, FVector(1, 0, 0)); //Back face
	BuildQuad(V4, V3, V8, V7, FVector(0, 0, 1)); //Top face
	BuildQuad(V6, V5, V2, V1, FVector(0, 0, -1)); //Bottom face
}
