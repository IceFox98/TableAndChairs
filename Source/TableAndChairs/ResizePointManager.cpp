// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizePointManager.h"
#include "ResizePoint.h"
#include "ResizableObject.h"

// Sets default values for this component's properties
UResizePointManager::UResizePointManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UResizePointManager::BeginPlay()
{
	Super::BeginPlay();

	SetupInputBinding();
	OnResizePointMovedDelegate.BindUObject(this, &UResizePointManager::OnPositionChecked);
}

void UResizePointManager::SetupInputBinding()
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//Setup input binding
	if (PlayerController && PlayerController->InputComponent)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->InputComponent->BindAction("RecordMovement", IE_Pressed, this, &UResizePointManager::StartRecordingMovement);
		PlayerController->InputComponent->BindAction("RecordMovement", IE_Released, this, &UResizePointManager::StopRecordingMovement);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController, or its InputComponent, is NULL"));
	}
}

// Called every frame
void UResizePointManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerController)
	{
		return;
	}

	if (bRecordingMovement) //Calculate movement
	{
		//Get camera position (player)
		FVector PlayerStartPosition;
		FRotator PlayerStartRotation;
		PlayerController->GetPlayerViewPoint(PlayerStartPosition, PlayerStartRotation);

		//Cast raycast, it will ignore everything but floor
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult);

		if (HitResult.ImpactPoint == FVector::ZeroVector)
		{
			return;
		}

		//Create a plane at ResizePoint position, and get the point where raycast intersect it, due to get the correct amount of movement
		FVector IntersectionPoint = FMath::LinePlaneIntersection(PlayerStartPosition, HitResult.ImpactPoint, StartHitPoint, FVector(0, 0, 1));

		if (IntersectionPoint == FVector::ZeroVector)
		{
			//Something went wrong
			UE_LOG(LogTemp, Warning, TEXT("NO INTERSECT"));
			StopRecordingMovement();
			return;
		}

		if (ResizePointHit)
		{
			//DrawDebugLine(GetWorld(), PlayerStartPosition, IntersectionPoint, FColor::Green, true);
			ResizePointHit->CheckPosition(IntersectionPoint);
		}
	}
}

void UResizePointManager::InitializePoints(const FVector &ParentExtent, USceneComponent* ParentComp)
{
	if (!ResizePointMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("ResizePointMesh is NULL."));
		return;
	}

	if (!ParentComp)
	{
		UE_LOG(LogTemp, Error, TEXT("The Parent Component is null. Unable to generate/update ResizePoints."));
		return;
	}

	const TArray<FVector> Positions = {
		FVector(-ParentExtent.X, -ParentExtent.Y, ParentExtent.Z),
		FVector(+ParentExtent.X, -ParentExtent.Y, ParentExtent.Z),
		FVector(-ParentExtent.X, +ParentExtent.Y, ParentExtent.Z),
		FVector(+ParentExtent.X, +ParentExtent.Y, ParentExtent.Z),
	};

	//Create Resize Points and set their start position
	for (int32 i = 0; i < Positions.Num(); i++)
	{
		UResizePoint* ResizePoint = NewObject<UResizePoint>(this, UResizePoint::StaticClass());

		if (!ResizePoint) 
		{
			continue;
		}

		ResizePoint->SetStaticMesh(ResizePointMesh);
		ResizePoint->SetRelativeScale3D(FVector(0.4, 0.4, 0.4));
		ResizePoint->SetupAttachment(ParentComp);
		ResizePoint->RegisterComponent();
		ResizePoint->SetRelativeLocation(Positions[i]);

		ResizePoints.Add(ResizePoint);
	}
}

void UResizePointManager::StartRecordingMovement()
{
	FHitResult HitResult;
	bool bHasHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (bHasHit)
	{
		const UPrimitiveComponent* ComponentHit = HitResult.GetComponent();
		bool bIsResizePoint = ComponentHit->IsA(UResizePoint::StaticClass());

		//Is the hit Resize Point attached to this table actor? Just for manage multiple instances of table
		bool bIsVaild = ComponentHit->IsAttachedTo(GetOwner()->GetRootComponent());

		if (bIsResizePoint && bIsVaild)
		{
			UE_LOG(LogTemp, Warning, TEXT("Start Recording"));

			const UResizePoint* ResizePointToFind = Cast<UResizePoint>(ComponentHit);
			const int32 Index = ResizePoints.IndexOfByKey(ResizePointToFind);

			if (Index == INDEX_NONE)
			{
				UE_LOG(LogTemp, Error, TEXT("Can't find ResizePoint in the array."));
				return;
			}

			ResizePointHit = ResizePoints[Index];

			StartHitPoint = HitResult.ImpactPoint;
			StartHitPoint.Z = ResizePointHit->GetComponentLocation().Z;

			bRecordingMovement = true;
		}
	}
}

void UResizePointManager::StopRecordingMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Recording"));

	bRecordingMovement = false;
	StartHitPoint = FVector::ZeroVector;
}

void UResizePointManager::OnPositionChecked(const bool IsValid, const UResizePoint *ResizePointRef, FVector &CheckedPosition)
{
	if (!IsValid)
	{
		UE_LOG(LogTemp, Error, TEXT("OnResizePointMoved: The position is not valid: %s"), *CheckedPosition.ToString());
		return;
	}

	//Get resizable object only
	IResizableObject* ResizableObject = Cast<IResizableObject>(GetOwner());

	if (!ResizableObject)
	{
		UE_LOG(LogTemp, Error, TEXT("This Object can't be resized."));
		return;
	}

	//Get the direction of the resize point
	float DirectionX = FMath::Sign(ResizePointRef->GetRelativeLocation().X);
	float DirectionY = FMath::Sign(ResizePointRef->GetRelativeLocation().Y);
	const FVector Direction(DirectionX, DirectionY, 1);

	ResizableObject->ClampSize(Direction, CheckedPosition);
	const FVector NewExtent = CheckedPosition - GetOwner()->GetActorLocation();

	const bool DoesIntersect = ResizableObject->DoesIntersect(Direction, NewExtent);

	if (DoesIntersect) //If, during resizing, you hit another actor, the resize stops
	{
		//UE_LOG(LogTemp, Error, TEXT("Intersect"));
		return;
	}

	//Resize Mesh and get the new center
	const FVector DeltaSize = ResizableObject->ResizeMesh(Direction, NewExtent);

	if (DeltaSize == FVector::ZeroVector) //If we aren't changing the size, we can avoid to update ResizePoint positions.
	{
		return;
	}

	//Set the ResizePoints position
	for (int32 i = 0; i < ResizePoints.Num(); i++)
	{
		UResizePoint* ResizePoint = ResizePoints[i];

		if (!ResizePoint)
		{
			continue;
		}

		const FVector CurrentResizePointPosition = ResizePoint->GetRelativeLocation();

		const float CurrXSign = FMath::Sign(CurrentResizePointPosition.X);
		const float CurrYSign = FMath::Sign(CurrentResizePointPosition.Y);

		//The other half of the movement was applied by the parent actor (ResizePoints are child of it)
		FVector DeltaPosition = (DeltaSize * Direction) * .5f;

		if (DirectionX != CurrXSign)
		{
			DeltaPosition.X *= -1;
		}

		if (DirectionY != CurrYSign)
		{
			DeltaPosition.Y *= -1;
		}

		ResizePoint->SetWorldLocation(ResizePoint->GetComponentLocation() + DeltaPosition);
	}
}