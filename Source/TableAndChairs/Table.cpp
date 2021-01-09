// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/TriggerVolume.h"
#include "Chair.h"
#include "TableLeg.h"
#include "ResizePoint.h"

// Sets default values
ATable::ATable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VertexCount = (6 * 4); //6 faces with 4 vertices each
	Vertices.AddUninitialized(VertexCount);

	TrianglesCount = (6 * 2 * 3); //2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);

	Width = 100;
	Length = 300;
	Height = 20;

	ResizePointsCount = 4;

	//UE_LOG(LogTemp, Warning, TEXT("ATable"));

	for (int32 i = 0; i < ResizePointsCount; i++)
	{
		UResizePoint* rp = CreateDefaultSubobject<UResizePoint>(TEXT("ResizePoint" + i));
		rp->SetupAttachment(Mesh);
		ResizePoints.Add(rp);
	}

}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	BuildMesh();

	FBox Bounds = Mesh->Bounds.GetBox();

	Mesh->Bounds.GetBox().GetCenter();
	ResizePoints[0]->SetRelativeLocation(FVector(Bounds.Min.X, Bounds.Min.Y, Bounds.Max.Z));
	ResizePoints[1]->SetRelativeLocation(FVector(Bounds.Min.X, Bounds.Max.Y, Bounds.Max.Z));
	ResizePoints[2]->SetRelativeLocation(FVector(Bounds.Max.X, Bounds.Min.Y, Bounds.Max.Z));
	ResizePoints[3]->SetRelativeLocation(FVector(Bounds.Max.X, Bounds.Max.Y, Bounds.Max.Z));

}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//GetWorld()->LineTraceSingleByChannel()
	
	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint();

	UpdateLegsTransform();

}

void ATable::BuildMesh()
{
	Super::BuildMesh();

	//6 sides on cube, 4 verts each (corners)

	FVector scale = FVector(Width, Length, Height);
	FVector position = FVector(0, 0, 100);

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

	FTransform* t = new FTransform(FRotator::ZeroRotator, position, scale);
	FMatrix m = t->ToMatrixWithScale();

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		//(Vertices[i] *= scale) + position;
		Vertices[i] = m.TransformPosition(Vertices[i]);
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

	for (int32 i = 0; i < legCount; i++)
	{
		ATableLeg* leg = GetWorld()->SpawnActor<ATableLeg>(ATableLeg::StaticClass());

		if (leg)
		{
			leg->GenerateMesh(TArray<FLinearColor>());
			Legs.Add(leg);
		}
	}

}

void ATable::UpdateLegsTransform()
{
	FBox Bounds = Mesh->Bounds.GetBox();
	int32 legWidth = Legs[0]->Width;
	int32 legLength = Legs[0]->Length;

	Legs[0]->SetActorLocation(FVector(Bounds.Min.X, Bounds.Max.Y, Bounds.Min.Z));
	Legs[1]->SetActorLocation(FVector(Bounds.Min.X, Bounds.Min.Y + legLength, Bounds.Min.Z));
	Legs[2]->SetActorLocation(FVector(Bounds.Max.X - legWidth, Bounds.Max.Y, Bounds.Min.Z));
	Legs[3]->SetActorLocation(FVector(Bounds.Max.X - legWidth, Bounds.Min.Y + legLength, Bounds.Min.Z));
}
