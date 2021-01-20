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


	/**
	 * Calls the function of UPorceduralMesh class to generate the mesh.
	 * @param VertexColors teeest test
	 */
	void GenerateMesh(TArray<FColor> InVertexColors);

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
		UMaterial* Material;

protected:

	virtual void BuildMesh();

	void UpdateMesh();

	/** The vertices of this mesh */
	TArray<FVector> Vertices;

	/** The triangles of this mesh */
	TArray<int32> Triangles;

	/** The normals of this mesh */
	TArray<FVector> Normals;

	/** The number of Vertices of the mesh, must be calculated */
	int32 VertexCount;

	/** The number of Triangles of the mesh, must be calculated */
	int32 TrianglesCount;

	void BuildCube(FVector MeshSize, FVector Position);

private:

	void BuildQuad(const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &Normals);

	/** The current index of Vertices array */
	int32 VertexIndex;

	/** The current index of Triangles array */
	int32 TrianglesIndex;

	TArray<FColor> VertexColors;

};
