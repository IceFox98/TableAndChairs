// Fill out your copyright notice in the Description page of Project Settings.


#include "ResizePointManager.h"
#include "ResizePoint.h"
#include "ResizableObject.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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

	// ...

	SetupInputBinding();

	OnResizePointMovedDelegate.BindUObject(this, &UResizePointManager::OnResizePointMoved);

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

		FVector MovementAmount = FVector::ZeroVector;

		if (IntersectionPoint != FVector::ZeroVector)
		{
			//DrawDebugLine(GetWorld(), PlayerStartPosition, IntersectionPoint, FColor::Green, true);
			MovementAmount = IntersectionPoint - StartHitPoint;
		}
		else
		{
			//Something went wrong
			UE_LOG(LogTemp, Warning, TEXT("NO INTERSECT"));
			StopRecordingMovement();
			return;
		}

		if (ResizePointHit)
		{
			const FVector NewLocation = StartResizePointPosition + MovementAmount;
			ResizePointHit->SetPosition(NewLocation);
		}
	}
}

void UResizePointManager::InitializePoints(const FVector &Center, const FVector &Extent)
{
	const TArray<FVector> Positions = {
		FVector(-Extent.X, -Extent.Y, Extent.Z),
		FVector(+Extent.X, -Extent.Y, Extent.Z),
		FVector(-Extent.X, +Extent.Y, Extent.Z),
		FVector(+Extent.X, +Extent.Y, Extent.Z),
	};

	for (int32 i = 0; i < Positions.Num(); i++)
	{
		UResizePoint* ResizePoint = NewObject<UResizePoint>(this, UResizePoint::StaticClass());

		ResizePoint->SetupAttachment(GetOwner()->GetRootComponent());
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

			bRecordingMovement = true;

			const UResizePoint* ResizePointToFind = Cast<UResizePoint>(ComponentHit);
			const int32 Index = ResizePoints.IndexOfByKey(ResizePointToFind);

			if (Index == INDEX_NONE)
			{
				UE_LOG(LogTemp, Error, TEXT("Can't find ResizePoint in the array"));
				return;
			}

			ResizePointHit = ResizePoints[Index];

			StartHitPoint = HitResult.ImpactPoint;
			StartHitPoint.Z = ResizePointHit->GetComponentLocation().Z;

			StartResizePointPosition = ResizePointHit->GetRelativeLocation();
		}
	}
}

void UResizePointManager::StopRecordingMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Recording"));

	bRecordingMovement = false;
}

void UResizePointManager::OnResizePointMoved(const FVector &ResizePointPosition)
{
	//UE_LOG(LogTemp, Warning, TEXT("OnResizePointMoved called. Movement Amount: %s"), *ResizePointPosition.ToString());

	TArray<FVector> WorldPositions = TArray<FVector>();

	for (int32 i = 0; i < ResizePoints.Num(); i++)
	{
		UResizePoint* ResizePoint = ResizePoints[i];
		const FVector CurrentResizePointPosition = ResizePoint->GetComponentLocation();
		WorldPositions.Add(CurrentResizePointPosition);
	}

	const FBox BoxForCenter(WorldPositions);

	const FVector StartCenter = BoxForCenter.GetCenter();

	WorldPositions.Empty();

	const float MovedXSign = FMath::Sign(ResizePointPosition.X - StartCenter.X);
	const float MovedYSign = FMath::Sign(ResizePointPosition.Y - StartCenter.Y);

	for (int32 i = 0; i < ResizePoints.Num(); i++)
	{
		UResizePoint* ResizePoint = ResizePoints[i];

		const FVector CurrentResizePointPosition = ResizePoint->GetRelativeLocation();

		const float CurrXSign = FMath::Sign(CurrentResizePointPosition.X - StartCenter.X);
		const float CurrYSign = FMath::Sign(CurrentResizePointPosition.Y - StartCenter.Y);

		if (MovedXSign == CurrXSign && MovedYSign == CurrYSign) //It's the same ResizePoint we've just moved
		{
			WorldPositions.Add(ResizePoint->GetComponentLocation());
			continue;
		}

		FVector NewPosition = ResizePointPosition;

		if (MovedXSign != CurrXSign)
		{
			NewPosition.X = CurrentResizePointPosition.X;
		}

		if (MovedYSign != CurrYSign)
		{
			NewPosition.Y = CurrentResizePointPosition.Y;
		}

		ResizePoint->SetRelativeLocation(NewPosition);

		WorldPositions.Add(ResizePoint->GetComponentLocation());
	}

	const FBox Box(WorldPositions);

	const FVector Center = Box.GetCenter();
	const FVector Extent = Box.GetSize();

	UE_LOG(LogTemp, Warning, TEXT("Center: %s"), *Center.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Extent: %s"), *Extent.ToString());

	if (GetOwner()->GetClass()->ImplementsInterface(UResizableObject::StaticClass()))
	{
		IResizableObject* Interface = Cast<IResizableObject>(GetOwner());
		bool Result = Interface->ResizeMesh(Center, Extent);

		if (!Result)
		{
			StopRecordingMovement();
		}

	}
}
