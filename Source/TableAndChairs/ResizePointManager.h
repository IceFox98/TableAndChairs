// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResizePointManager.generated.h"

class UResizePoint;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TABLEANDCHAIRS_API UResizePointManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResizePointManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializePoints(const FVector &Center, const FVector &Extent);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<UResizePoint*> ResizePoints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UResizePoint* Test;

};
