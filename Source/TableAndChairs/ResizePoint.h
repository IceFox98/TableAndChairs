// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ResizePoint.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TABLEANDCHAIRS_API UResizePoint : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResizePoint();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	/**
	 * Checks the passed position and calls the delegate OnResizePointMovedDelegate
	 * @param NewPosition - The position that will be checked and passed into the delegate
	 */
	void CheckPosition(FVector &NewPosition) const;

private:

	UPROPERTY()
		class UResizePointManager* ResizePointManager;
};
