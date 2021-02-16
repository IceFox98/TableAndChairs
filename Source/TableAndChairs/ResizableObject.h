// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ResizableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UResizableObject : public UInterface
{
	GENERATED_BODY()
};

class TABLEANDCHAIRS_API IResizableObject
{
	GENERATED_BODY()

public:

	UFUNCTION()
		virtual FVector ResizeMesh(const FVector &Direction, const FVector &NewExtent) { return FVector::ZeroVector; };

	UFUNCTION()
		virtual FVector ClampSize(const FVector &Direction, const FVector &SizeToCheck) { return FVector::ZeroVector; };

	UFUNCTION()
		virtual bool DoesIntersect(const FVector &Direction, const FVector &Extent) { return true; };

	UFUNCTION()
		virtual FVector GetMeshSize() { return FVector::ZeroVector; };
};
