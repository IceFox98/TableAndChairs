// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

APlayerCamera::APlayerCamera()
{
	GetCameraComponent()->SetAspectRatio(2.35f);
	GetCameraComponent()->SetFieldOfView(75.0f);
}

void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->SetViewTarget(this);
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->bEnableClickEvents = true;

		if (PlayerController->InputComponent)
		{
			PlayerController->InputComponent->BindAxis("MoveForward", this, &APlayerCamera::MoveForward);
			PlayerController->InputComponent->BindAxis("MoveRight", this, &APlayerCamera::MoveRight);
		}
	}
}

void APlayerCamera::MoveForward(float InputAxis)
{
	const FVector MovementAmount = FVector::ForwardVector * InputAxis * MovementSpeed;

	//Due to ignore the current rotation
	SetActorLocation(GetActorLocation() + MovementAmount);
}

void APlayerCamera::MoveRight(float InputAxis)
{
	const FVector MovementAmount = FVector::RightVector * InputAxis * MovementSpeed;

	AddActorLocalOffset(MovementAmount, true);
}
