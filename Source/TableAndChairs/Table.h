// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResizableObject.h"
#include "Table.generated.h"

class UDynamicMeshComponent;
class UResizePointManager;

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

	void BuildMesh(const FVector &Center, const FVector &Extent);

	UPROPERTY(EditDefaultsOnly)
		FVector Size;

	UPROPERTY(EditDefaultsOnly)
		FVector MaxSize;

	UPROPERTY(EditDefaultsOnly)
		FVector LegSize;

	virtual bool ResizeMesh(const FVector &Center, const FVector &Extent) override;

	//UPROPERTY(EditDefaultsOnly)
	//	FVector Size;
private:

	UPROPERTY(VisibleAnywhere)
		UDynamicMeshComponent* DynamicMeshComponent;

	UPROPERTY(VisibleAnywhere)
		UResizePointManager* ResizePointManager;
};
