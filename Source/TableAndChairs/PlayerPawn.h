// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "PlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TABLEANDCHAIRS_API APlayerPawn : public ADefaultPawn
{
	GENERATED_BODY()

public:

	APlayerPawn();

protected:

	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* PlayerCameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* PlayerSpringArm;

	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float ZoomSpeed;

	/** The minimun length of the Spring Arm */
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float ZoomMinValue;

	/** The maximum length of the Spring Arm */
	UPROPERTY(EditAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
		float ZoomMaxValue;

	/** Increases/Decreases the length of the Spring Arm */
	void Zoom(float InputAxis);
};
