// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TableAndChairsGameModeBase.h"
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

	virtual void HandleDestruction();

	/**
	 * Calls the function of UPorceduralMesh class to generate the mesh.
	 */
	void GenerateMesh();

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
		UMaterial* Material;

protected:

	ATableAndChairsGameModeBase* GameMode;

	/** The Vertices of this mesh */
	TArray<FVector> Vertices;

	virtual void BuildMesh();

	/** Updates the first section of this mesh, using the values of member variables */
	void UpdateMesh();

	/**
	 * Adds the Vertices, Triangles and Normals necessary to create the mesh
	 * @param MeshSize - The size of the Cube
	 * @param Position - Where the mesh will be positioned. NOTE: Its starting pivot will be always (0, 0, 0)
	 */
	void BuildCube(const FVector &MeshSize, const FVector &Position, const FColor &Color);

	/** Removes all sections of this Mesh and resets Vertices, Triangles, Normals, ... */
	void ResetBuffers();

private:

	void BuildQuad(const FVector &BottomLeft, const FVector &BottomRight, const FVector &TopRight, const FVector &TopLeft, const FVector &Normal, const FVector &InTangent, const FColor &Color);

	/** The current index of Vertices array */
	int32 VertexIndex;

	/** The current index of Triangles array */
	int32 TrianglesIndex;

	/** The Triangles of this mesh */
	TArray<int32> Triangles;

	/** The Normals of this mesh */
	TArray<FVector> Normals;

	/** The Tangents of this mesh */
	TArray<FProcMeshTangent> Tangents;

	/** The UVs of this mesh */
	TArray<FVector2D> UVs;

	/** The VertexColors of this mesh */
	TArray<FColor> VertexColors;

};
