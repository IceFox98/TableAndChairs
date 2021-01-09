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

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(Category = CameraMovement, EditAnywhere)
		float MovementSpeed = 10.0f;

	void MoveForward(float inputAxis);
	void MoveRight(float inputAxis);



};
