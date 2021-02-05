// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResizableObject.h"
#include "Table.generated.h"

class UDynamicMeshComponent;
class UResizePointManager;
class ULegsManager;

UCLASS()
class TABLEANDCHAIRS_API ATable : public AActor, public IResizableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BuildMesh(const FVector &Center, const FVector &MeshSize, const bool Update);

	UPROPERTY(EditDefaultsOnly)
		FVector Size;

	UPROPERTY(EditDefaultsOnly)
		FVector MaxSize;

	UPROPERTY(EditDefaultsOnly)
		FVector MinSize;

	virtual FVector ResizeMesh(const FVector &Direction, const FVector &DeltaSize) override;

	virtual FVector ClampSize(const FVector &Direction, const FVector &SizeToCheck) override;

private:

	UPROPERTY(VisibleAnywhere)
		UDynamicMeshComponent* DynamicMeshComponent;

	UPROPERTY(VisibleAnywhere)
		UResizePointManager* ResizePointManager;

	UPROPERTY(VisibleAnywhere)
		ULegsManager* LegsManager;
};
