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
			PlayerController->InputComponent->BindAxis("Rotate", this, &APlayerCamera::Rotate);
		}
	}
}

void APlayerCamera::MoveForward(float InputAxis)
{
	if (InputAxis == 0)
	{
		return;
	}

	FVector CameraLocation = GetActorLocation();

	FVector NewLocation = CameraLocation + (GetActorForwardVector() * InputAxis * MovementSpeed * GetWorld()->DeltaTimeSeconds);
	NewLocation.Z = CameraLocation.Z;

	SetActorLocation(NewLocation);
}

void APlayerCamera::MoveRight(float InputAxis)
{
	if (InputAxis == 0)
	{
		return;
	}

	FVector CameraLocation = GetActorLocation();

	FVector NewLocation = CameraLocation + (GetActorRightVector() * InputAxis * MovementSpeed * GetWorld()->DeltaTimeSeconds);
	NewLocation.Z = CameraLocation.Z;

	SetActorLocation(NewLocation);
}

void APlayerCamera::Rotate(float InputAxis)
{
	if (InputAxis == 0)
	{
		return;
	}

	FRotator CameraRotation = GetActorRotation();

	float RotateAmount = InputAxis * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	CameraRotation.Yaw += RotateAmount;

	SetActorRotation(CameraRotation);
}
