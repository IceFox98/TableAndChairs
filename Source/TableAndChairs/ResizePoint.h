// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResizePoint.generated.h"

UCLASS()
class TABLEANDCHAIRS_API AResizePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AResizePoint();

private:

	/** The mesh associated with this Pawn. */
	UPROPERTY(Category = ResizePoint, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComponent;
};
