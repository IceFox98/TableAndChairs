// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TableAndChairsGameModeBase.h"
#include "EngineUtils.h"
#include "ResizableObject.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralMeshComponent.h"

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
	ZoomMinValue = 200.f;
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

	GameMode = Cast<ATableAndChairsGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
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

void APlayerPawn::Zoom(float ZoomDirection)
{
	if (ZoomDirection == 0.f || !PlayerSpringArm)
	{
		return;
	}

	const float ZoomAmount = PlayerSpringArm->TargetArmLength += (ZoomDirection * ZoomSpeed);
	const float ZoomAmountClamped = FMath::Clamp(ZoomAmount, ZoomMinValue, ZoomMaxValue);

	PlayerSpringArm->TargetArmLength = ZoomAmountClamped;
}

void APlayerPawn::SpawnNewMesh()
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is NULL. Unable to spawn new mesh."));
		return;
	}

	if (!GameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode is NULL. Unable to spawn new mesh."));
		return;
	}

	if (!GameMode->ActorToSpawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorToSpawn is NULL. Unable to spawn it."));
		return;
	}

	FHitResult HitResult;
	bool bHasHit = PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult);

	if (bHasHit)
	{
		IResizableObject* ResizableObject = Cast<IResizableObject>(GameMode->ActorToSpawn->GetDefaultObject());

		//Get resizable object only
		if (!ResizableObject)
		{
			UE_LOG(LogTemp, Error, TEXT("This Object can't be spawned."));
			return;
		}

		FVector SpawnPoint = HitResult.ImpactPoint;

		//Create a Box of the table that should be spawned
		const float OffsetBetweenTables = 100.f;
		const FVector TableExtent = (ResizableObject->GetMeshSize() * .5f) + OffsetBetweenTables;

		TArray<AActor*> OverlappedActors;
		TArray<AActor*> ActorsToIgnore = { HitResult.GetActor() }; //Ignore floor
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

		const bool bDoesIntersect = UKismetSystemLibrary::BoxOverlapActors(GetWorld(), SpawnPoint, TableExtent, ObjectTypes, nullptr, ActorsToIgnore, OverlappedActors);

		if (!bDoesIntersect) //If everything is ok, i can spawn
		{
			GetWorld()->SpawnActor<AActor>(GameMode->ActorToSpawn, SpawnPoint, FRotator::ZeroRotator);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Can't spawn. The Actor intersects with another one."));
		}
	}
}

void APlayerPawn::RemoveMesh()
{
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController is NULL. Unable to remove meshes."));
		return;
	}

	FHitResult HitResult;
	bool bHasHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (bHasHit)
	{
		AActor* ActorHit = HitResult.GetActor();

		const bool bCanBeRemoved = ActorHit->Implements<UResizableObject>();

		if (bCanBeRemoved)
		{
			//TODO: Implements Object Pooling (?)
			ActorHit->Destroy();
		}
	}
}
