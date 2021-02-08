// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LegsManager.generated.h"

class ULeg;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TABLEANDCHAIRS_API ULegsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULegsManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Generates the legs component and adds them to the array */
	void BuildLegs();

	/**
	 * Updates the legs position depending on the passed extent.
	 */
	void UpdateLegsPosition(const FVector &ParentExtent);

	FVector GetLegSize();

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector LegSize;

	UPROPERTY(VisibleAnywhere)
		TArray<ULeg*> Legs;
};
