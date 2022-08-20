// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LegsManager.generated.h"

class UProceduralMeshComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TABLEANDCHAIRS_API ULegsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULegsManager();

public:	

	/**
	 * Generates the legs component and adds them to the array.
	 * @param ParentComp - The Component whose legs will be attached to
	 */
	void BuildLegs(USceneComponent* ParentComp);

	/**
	 * Updates the legs position depending on the passed extent.
	 * @param ParentExtent - The extent of the parent
	 */
	void UpdateLegsPosition(const FVector &ParentExtent);

	/** Returns the starting size of the leg */
	FVector GetLegSize() const;

private:

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		FVector LegSize;

	/** The Array which contains each spawned legs */
	UPROPERTY(VisibleAnywhere)
		TArray<UProceduralMeshComponent*> Legs;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UMaterial* LegMaterial;
};
