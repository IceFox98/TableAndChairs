// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMesh.h"
#include "Chair.h"
#include "TableLeg.h"
#include "Table.generated.h"

UCLASS()
class TABLEANDCHAIRS_API ATable : public ADynamicMesh
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void BuildMesh() override;

	TArray<AChair*> Chairs;
	TArray<ATableLeg*> Legs;

	//TMap<AActor, ADynamicMesh> ResizePoints;

	//TArray<FVector> Vertices;
	//TArray<int32> Triangles;

	//int32 VertexCount = 0;
	//int32 VertexIndex = 0;

	//int32 TrianglesCount = 0;
	//int32 TrianglesIndex = 0;

	//void BuildQuad(const FVector bottomLeft, const FVector bottomRight, const FVector topRight, const FVector topLeft);
};
