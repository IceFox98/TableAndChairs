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

	/**
	 * Calls the function of UPorceduralMesh class to generate the mesh.
	 * @param VertexColors teeest test
	 */
	void GenerateMesh(TArray<FLinearColor> VertexColors);

	//Unit amount on X-Axis
	int32 Width = 0;
	//Unit amount on Y-Axis
	int32 Length = 0;
	//Unit amount on Z-Axis
	int32 Height = 0;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* Mesh;

protected:

	virtual void BuildMesh();

	TArray<int32> Triangles;

	int32 VertexCount = 0;
	int32 VertexIndex = 0;

	int32 TrianglesCount = 0;
	int32 TrianglesIndex = 0;

	void BuildQuad(const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft);
	void BuildCube(const FVector &FrontBottomLeft, const FVector &FrontBottomRight, const FVector &FrontTopRight, const FVector &FrontTopLeft, const FVector &BackBottomLeft, const FVector &BackBottomRight, const FVector &BackTopRight, const FVector &BackTopLeft);
};
