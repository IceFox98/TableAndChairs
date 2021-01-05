// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
//#include "DynamicMesh.h"

// Sets default values
ATable::ATable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);*/

}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	VertexCount = (6 * 4); //6 faces with 4 vertices each
	Vertices.AddUninitialized(VertexCount);

	TrianglesCount = (6 * 2 * 3); //2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);

	Width = 300;
	Length = 100;
	Height = 20;

	BuildMesh();
}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATable::BuildMesh()
{
	Super::BuildMesh();

	//6 sides on cube, 4 verts each (corners)

	//FVector scale = FVector(3, 1, 0.2);
	FVector position = FVector(-50, 50, 100);

	FVector V1 = FVector(0, 0, 0); //Front - Bottom Left
	FVector V2 = FVector(1, 0, 0); //Front - Bottom Right
	FVector V3 = FVector(1, 0, 1); //Front - Top Right
	FVector V4 = FVector(0, 0, 1); //Front - Top Left
	FVector V5 = FVector(1, -1, 0); //Back - Bottom Left
	FVector V6 = FVector(0, -1, 0); //Back - Bottom Right
	FVector V7 = FVector(0, -1, 1); //Back - Top Right
	FVector V8 = FVector(1, -1, 1); //Back - Top Left

	BuildQuad(V1, V2, V3, V4); //Front Face
	BuildQuad(V2, V5, V8, V3); //Right Face
	BuildQuad(V6, V1, V4, V7); //Left Face
	BuildQuad(V5, V6, V7, V8); //Back Face
	BuildQuad(V4, V3, V8, V7); //Top Face
	BuildQuad(V6, V5, V2, V1); //Bottom Face

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		//(Vertices[i] *= scale) + position;
		(Vertices[i] *= FVector(Width, Length, Height)) + position;
	}

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	GenerateMesh(VertexColors);

	const int32 legCount = 4;

	TArray<FVector> legPositions = { FVector(0, 0, 0), FVector(0, 0, 0), FVector(0, 0, 0), FVector(0, 0, 0) };

	for (int32 i = 0; i < legCount; i++)
	{
		ATableLeg* leg = GetWorld()->SpawnActor<ATableLeg>(ATableLeg::StaticClass());

		for (int32 k = 0; k < leg->Vertices.Num(); k++)
		{
			leg->Vertices[k] += FVector(100 * i, 0, 0);
		}

		leg->GenerateMesh(TArray<FLinearColor>());
		Legs.Add(leg);
	}

}
