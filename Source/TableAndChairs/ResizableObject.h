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

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
		virtual FVector ResizeMesh(const FVector &Direction, const FVector &DeltaSize) { return FVector::ZeroVector; };

	UFUNCTION()
		virtual FVector ClampSize(const FVector &Direction, const FVector &SizeToCheck) { return FVector::ZeroVector; };
};
