// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "PlayerCamera.generated.h"

/**
 * 
 */
UCLASS()
class TABLEANDCHAIRS_API APlayerCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	APlayerCamera();

	virtual void BeginPlay() override;

public:

	UPROPERTY(Category = CameraMovement, EditAnywhere)
		float MovementSpeed = 1500.0f;

	UPROPERTY(Category = CameraMovement, EditAnywhere)
		float RotateSpeed = 70.0f;

	void MoveForward(float InputAxis);
	void MoveRight(float InputAxis);
	
	void Rotate(float InputAxis);
};
