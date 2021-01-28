// Fill out your copyright notice in the Description page of Project Settings.

#include "DynamicMesh.h"

// Sets default values
ADynamicMesh::ADynamicMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

	GameMode = Cast<ATableAndChairsGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ADynamicMesh::HandleDestruction()
{
	Destroy();
}

void ADynamicMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADynamicMesh::BuildMesh()
{
	//Nothing
}

void ADynamicMesh::GenerateMesh()
{
	Mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);

	if (Material)
	{
		Mesh->SetMaterial(0, Material);
	}
}

void ADynamicMesh::UpdateMesh()
{
	Mesh->UpdateMeshSection(0, Vertices, Normals, UVs, VertexColors, Tangents);
}

void ADynamicMesh::BuildQuad(const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &InNormal, const FVector &InTangent, const FColor &Color)
{
	const int32 VertexCount = 4; //4 vertices for each quad
	const int32 TrianglesCount = 2 * 3; //2 triangles per quad, 3 vertices each

	Vertices.AddUninitialized(VertexCount);
	Triangles.AddUninitialized(TrianglesCount);
	Normals.AddUninitialized(VertexCount);
	VertexColors.AddUninitialized(VertexCount);
	UVs.AddUninitialized(VertexCount);
	Tangents.AddUninitialized(VertexCount);

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

	VertexColors[Index1] = Color;
	VertexColors[Index2] = Color;
	VertexColors[Index3] = Color;
	VertexColors[Index4] = Color;

	//Unreal's UVs start from upper-left and proceeds clockwise
	UVs[Index1] = FVector2D(0.f, 1.f);
	UVs[Index2] = FVector2D(1.f, 1.f);
	UVs[Index3] = FVector2D(1.f, 0.f);
	UVs[Index4] = FVector2D(0.f, 0.f);

	Triangles[TrianglesIndex++] = Index1;
	Triangles[TrianglesIndex++] = Index2;
	Triangles[TrianglesIndex++] = Index3;
	Triangles[TrianglesIndex++] = Index1;
	Triangles[TrianglesIndex++] = Index3;
	Triangles[TrianglesIndex++] = Index4;

	// In a cube all vertices of a face have the same normals and tangents
	Normals[Index1] = Normals[Index2] = Normals[Index3] = Normals[Index4] = InNormal;
	Tangents[Index1] = Tangents[Index2] = Tangents[Index3] = Tangents[Index4] = FProcMeshTangent(InTangent, false);
}

void ADynamicMesh::BuildCube(const FVector &MeshSize, const FVector &Position, const FColor &Color)
{
	// Divide in half the offset to get center of object
	const float OffsetX = MeshSize.X * .5f;
	const float OffsetY = MeshSize.Y * .5f;
	const float OffsetZ = MeshSize.Z * .5f;

	FVector V1 = FVector(-OffsetX, -OffsetY, -OffsetZ) + Position; //Front - Bottom Left
	FVector V2 = FVector(-OffsetX, OffsetY, -OffsetZ) + Position; //Front - Bottom Right
	FVector V3 = FVector(-OffsetX, OffsetY, OffsetZ) + Position; //Front - Top Right
	FVector V4 = FVector(-OffsetX, -OffsetY, OffsetZ) + Position; //Front - Top Left
	FVector V5 = FVector(OffsetX, OffsetY, -OffsetZ) + Position; //Back - Bottom Left
	FVector V6 = FVector(OffsetX, -OffsetY, -OffsetZ) + Position; //Back - Bottom Right
	FVector V7 = FVector(OffsetX, -OffsetY, OffsetZ) + Position; //Back - Top Right
	FVector V8 = FVector(OffsetX, OffsetY, OffsetZ) + Position; //Back - Top Left

	//Tangent is the right vector of the normal
	BuildQuad(V1, V2, V3, V4, FVector(-1, 0, 0), FVector(0, -1, 0), Color); //Front face
	BuildQuad(V6, V1, V4, V7, FVector(0, -1, 0), FVector(1, 0, 0), Color); //Left face
	BuildQuad(V2, V5, V8, V3, FVector(0, 1, 0), FVector(-1, 0, 0), Color); //Right face
	BuildQuad(V5, V6, V7, V8, FVector(1, 0, 0), FVector(0, 1, 0), Color); //Back face
	BuildQuad(V4, V3, V8, V7, FVector(0, 0, 1), FVector(0, 1, 0), Color); //Top face
	BuildQuad(V6, V5, V2, V1, FVector(0, 0, -1), FVector(0, -1, 0), Color); //Bottom face
}

void ADynamicMesh::ResetBuffers()
{
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVs.Empty();
	VertexColors.Empty();
	Tangents.Empty();

	VertexIndex = 0;
	TrianglesIndex = 0;

	Mesh->ClearAllMeshSections();
}
