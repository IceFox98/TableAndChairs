// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Table.h"
#include "EngineUtils.h"

APlayerPawn::APlayerPawn()
{
	//Remap the default key binding
	bAddDefaultMovementBindings = false;

	PlayerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerSpringArm"));
	PlayerSpringArm->SetupAttachment(RootComponent);
	PlayerSpringArm->TargetArmLength = 800.f;
	PlayerSpringArm->SetRelativeRotation(FRotator(-50.f, 0.f, 0.f));

	// Move Spring arm with character only on yaw rotation
	PlayerSpringArm->bUsePawnControlRotation = true;
	PlayerSpringArm->bInheritPitch = false;
	PlayerSpringArm->bInheritRoll = false;
	PlayerSpringArm->bInheritYaw = true;

	// Enables camera lag
	PlayerSpringArm->bEnableCameraLag = true;
	PlayerSpringArm->bEnableCameraRotationLag = true;

	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCameraComponent"));
	PlayerCameraComponent->SetupAttachment(PlayerSpringArm);
	PlayerCameraComponent->bUsePawnControlRotation = false; // This lets the camera looks at the origin of the spring arm, following its rotation

	ZoomSpeed = 20.f;
	ZoomMinValue = 400.f;
	ZoomMaxValue = 1200.f;
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetController<APlayerController>();

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
	}

	//GameMode = Cast<ATableAndChairsGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &ADefaultPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADefaultPawn::MoveRight);
	PlayerInputComponent->BindAxis("Zoom", this, &APlayerPawn::Zoom);
	PlayerInputComponent->BindAxis("Rotate", this, &ADefaultPawn::TurnAtRate);
	PlayerInputComponent->BindAction("SpawnNewMesh", IE_Pressed, this, &APlayerPawn::SpawnNewMesh);
	PlayerInputComponent->BindAction("RemoveMesh", IE_Pressed, this, &APlayerPawn::RemoveMesh);
}

void APlayerPawn::Zoom(float InputAxis)
{
	if (InputAxis == 0.f || !PlayerSpringArm)
	{
		return;
	}

	const float ZoomAmount = PlayerSpringArm->TargetArmLength += (InputAxis * ZoomSpeed);
	const float ZoomAmountClamped = FMath::Clamp(ZoomAmount, ZoomMinValue, ZoomMaxValue);

	PlayerSpringArm->TargetArmLength = ZoomAmountClamped;
}

void APlayerPawn::SpawnNewMesh()
{

}

void APlayerPawn::RemoveMesh()
{

}
