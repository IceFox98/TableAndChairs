// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DynamicMeshComponent.h"
#include "Leg.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TABLEANDCHAIRS_API ULeg : public UDynamicMeshComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULeg(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * Creates a leg (cube) with the size indicated and moves it at 'Center' position.
	 * @param Center - The position where the actor will be moved
	 */
	void BuildMesh(const FVector &Center, const FVector &Size);
};
