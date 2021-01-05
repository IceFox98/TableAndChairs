// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "DynamicMesh.generated.h"

UCLASS()
class TABLEANDCHAIRS_API ADynamicMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	TArray<FVector> Vertices;

	void GenerateMesh(TArray<FLinearColor> VertexColors);

protected:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* Mesh;

	virtual void BuildMesh();

	TArray<int32> Triangles;

	int32 VertexCount = 0;
	int32 VertexIndex = 0;

	int32 TrianglesCount = 0;
	int32 TrianglesIndex = 0;

	int32 Width = 0;
	int32 Length = 0;
	int32 Height = 0;

	void BuildQuad(const FVector bottomLeft, const FVector bottomRight, const FVector topRight, const FVector topLeft);
};
