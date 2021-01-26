// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
#include "Chair.h"
#include "TableLeg.h"
#include "ResizePoint.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATable::ATable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ChairsOnAxis.Add(EAxes::X);
	ChairsOnAxis.Add(EAxes::Y);

	LegsCount = 4;
	ResizePointsCount = 4;

	bRecordingMovement = false;
	DistanceFromTable = .25f;
	ChairOffset = 15.f;
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	if (GameMode)
	{
		//Calculating the right value of ChairOffsetZ 
		Size = GameMode->TableSize;

		FVector ChairSeatSize = GameMode->ChairSeatSize;
		ChairWidthWithOffset = ChairSeatSize.X + (ChairOffset * 2);

		float ChairLegSeatHeight = ChairSeatSize.Z + GameMode->ChairLegSize.Z;

		ChairOffsetZ = (ChairSeatSize.Z * .5f) + (GameMode->TableLegSize.Z - ChairLegSeatHeight);
		//ChairOffsetZ = 0;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GameMode is NULL"));
		return;
	}

	SpawnTableLegs();
	SpawnResizePoints();

	BuildMesh();
	UpdateResizePointsTransform();
	UpdateLegsTransform();
	CalculateChairs();

	SetupInputBinding();
}

void ATable::SpawnTableLegs()
{
	if (!TableLegClass)
	{
		UE_LOG(LogTemp, Error, TEXT("TableLegClass is NULL. Unable to spawn the table legs!"));
		return;
	}

	for (int32 i = 0; i < LegsCount; i++)
	{
		ATableLeg* LegInstance = GetWorld()->SpawnActor<ATableLeg>(TableLegClass);

		if (LegInstance)
		{
			LegInstance->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Legs.Add(LegInstance);
		}
	}
}

void ATable::SpawnResizePoints()
{
	//Generate ResizePoints
	for (int32 i = 0; i < ResizePointsCount; i++)
	{
		AResizePoint* ResizePointInstance = GetWorld()->SpawnActor<AResizePoint>(AResizePoint::StaticClass());

		if (ResizePointInstance)
		{
			ResizePointInstance->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			ResizePoints.Add(ResizePointInstance);
		}
	}
}

void ATable::SetupInputBinding()
{
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//Setup input binding
	if (PlayerController && PlayerController->InputComponent)
	{
		PlayerController->InputComponent->BindAction("RecordMovement", IE_Pressed, this, &ATable::StartRecordingMovement);
		PlayerController->InputComponent->BindAction("RecordMovement", IE_Released, this, &ATable::StopRecordingMovement);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController, or its InputComponent, is NULL"));
	}
}

void ATable::BuildMesh()
{
	Super::BuildMesh();

	const float LegHeight = GameMode->TableLegSize.Z;
	const FVector Position = FVector(0, 0, LegHeight + (Size.Z * .5f));

	BuildCube(Size, FVector::ZeroVector, FColor::Red);

	//FTransform* TableTransform = new FTransform(FRotator::ZeroRotator, Position, Scale);
	//FMatrix TableMatrix = TableTransform->ToMatrixWithScale();
	//Vertices[i] = TableMatrix.TransformPosition(Vertices[i]);

	GenerateMesh();
	SetActorLocation(Position);
}

// Called every frame
void ATable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		FVector IntersectionPoint;
		FPlane Plane = FPlane(StartHitPoint, FVector(0, 0, 1));
		bool bIntersect = FMath::SegmentPlaneIntersection(PlayerStartPosition, HitResult.ImpactPoint, Plane, IntersectionPoint);

		FVector MovementAmount = FVector::ZeroVector;

		if (bIntersect)
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

		//UE_LOG(LogTemp, Warning, TEXT("Recording - Start: %s - Hit: %s - MovementAmount: %s"), *StartHitPoint.ToString(), *HitResult.ImpactPoint.ToString(), *MovementAmount.ToString());

		UpdateTableMesh(MovementAmount);
		UpdateResizePointsTransform();
		UpdateLegsTransform();
		CalculateChairs();
	}
}

void ATable::UpdateTableMesh(const FVector &MovementAmount)
{
	//Get the sign to know the direction of the movement
	float XSign = FMath::Sign(StartHitPoint.X - StartCenter.X);
	float YSign = FMath::Sign(StartHitPoint.Y - StartCenter.Y);

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		FVector CurrMovementAmount = MovementAmount;
		FVector CurrVertex = StartVertices[i];

		//Freeze the vertex location 
		if (FMath::Sign(CurrVertex.X - StartCenter.X) != XSign)
		{
			CurrMovementAmount.X = 0;
		}

		if (FMath::Sign(CurrVertex.Y - StartCenter.Y) != YSign)
		{
			CurrMovementAmount.Y = 0;
		}

		Vertices[i] = CurrVertex + FVector(CurrMovementAmount.X, CurrMovementAmount.Y, 0);
	}

	//Updating Mesh
	UpdateMesh();
}

void ATable::UpdateLegsTransform()
{
	if (Legs.Num() == 0)
	{
		return;
	}

	const FVector LegSize = GameMode->TableLegSize;
	const FBox TableBounds = Mesh->Bounds.GetBox();

	const float ZPosition = TableBounds.Min.Z - (LegSize.Z * .5f);

	TArray<FVector> LegsPositions = {
		FVector(TableBounds.Min.X + (LegSize.X * .5f), TableBounds.Min.Y + (LegSize.Y * .5f), ZPosition),
		FVector(TableBounds.Min.X + (LegSize.X * .5f), TableBounds.Max.Y - (LegSize.Y * .5f), ZPosition),
		FVector(TableBounds.Max.X - (LegSize.X * .5f), TableBounds.Min.Y + (LegSize.Y * .5f), ZPosition),
		FVector(TableBounds.Max.X - (LegSize.X * .5f), TableBounds.Max.Y - (LegSize.Y * .5f), ZPosition)
	};

	UpdateTransforms((TArray<AActor*>)Legs, LegsPositions);
}

void ATable::UpdateResizePointsTransform()
{
	if (ResizePoints.Num() == 0)
	{
		return;
	}

	const FBox TableBounds = Mesh->Bounds.GetBox();

	TArray<FVector> ResizePointsPositions = {
		FVector(TableBounds.Min.X, TableBounds.Min.Y, TableBounds.Max.Z),
		FVector(TableBounds.Min.X, TableBounds.Max.Y, TableBounds.Max.Z),
		FVector(TableBounds.Max.X, TableBounds.Min.Y, TableBounds.Max.Z),
		FVector(TableBounds.Max.X, TableBounds.Max.Y, TableBounds.Max.Z)
	};

	UpdateTransforms((TArray<AActor*>)ResizePoints, ResizePointsPositions);
}

void ATable::UpdateTransforms(TArray<AActor*> ActorsToUpdate, const TArray<FVector> &NewPositions)
{
	if (ActorsToUpdate.Num() != NewPositions.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Func UpdateTransforms(): First and second arrays must have the same number of elements! Actors' transform will not be updated."));
		return;
	}

	for (int i = 0; i < NewPositions.Num(); i++)
	{
		FVector NewPosition = NewPositions[i];
		ActorsToUpdate[i]->SetActorLocation(NewPosition);
	}
}

void ATable::CalculateChairs()
{
	//const FBox Bounds = Mesh->bounds
	const FBox Bounds = Mesh->Bounds.GetBox();
	const FVector TableSize = Bounds.GetSize();

	const FVector TableLegSize = GameMode->TableLegSize;

	const float TableSizeAvailableX = TableSize.X - (TableLegSize.X * 2);
	const float TableSizeAvailableY = TableSize.Y - (TableLegSize.Y * 2);

	const int32 ChairsPerSideX = FMath::FloorToInt(TableSizeAvailableX / ChairWidthWithOffset);
	const int32 ChairsPerSideY = FMath::FloorToInt(TableSizeAvailableY / ChairWidthWithOffset);

	//VERTICAL SIDES - Top and Bottom - Need to flip X-Axis
	float TotalChairLength = GetTotalChairLength(ChairsPerSideY, TableSizeAvailableY);

	FVector StartSpawnPoint = FVector(Bounds.GetCenter().X, Bounds.Max.Y - TableLegSize.Y, Bounds.Min.Z); // Starting from right-center of table
	FVector SpawnOffset = FVector((TableSize.X  * .5f) + DistanceFromTable, (-TotalChairLength * .5f), -ChairOffsetZ);

	CalculateChairsOfAxis(StartSpawnPoint, SpawnOffset, ChairsPerSideY, TotalChairLength, EAxes::X);

	//HORIZONTAL SIDES - Right and Left - Need to flip Y-Axis
	TotalChairLength = GetTotalChairLength(ChairsPerSideX, TableSizeAvailableX);

	StartSpawnPoint = FVector(Bounds.Max.X - TableLegSize.X, Bounds.GetCenter().Y, Bounds.Min.Z); // Starting from top-center of table
	SpawnOffset = FVector((-TotalChairLength * .5f), (TableSize.Y  * .5f) + DistanceFromTable, -ChairOffsetZ);

	CalculateChairsOfAxis(StartSpawnPoint, SpawnOffset, ChairsPerSideX, TotalChairLength, EAxes::Y);
}

float ATable::GetTotalChairLength(const int ChairsPerSide, const float TableSideLength) const
{
	if (ChairsPerSide == 0) //Can't divide by 0
	{
		return 0;
	}

	float TotalChairsLength = ChairsPerSide * ChairWidthWithOffset;
	float AdditionalChairOffset = (TableSideLength - TotalChairsLength) / ChairsPerSide;

	return ChairWidthWithOffset + AdditionalChairOffset;
}

void ATable::CalculateChairsOfAxis(FVector StartSpawnPoint, FVector SpawnOffset, const int ChairsPerSide, const float TotalChairLength, const EAxes FlipAxis)
{
	//Get the rotation of the table
	float InitialRotation = GetActorRotation().Yaw;

	if (FlipAxis == EAxes::X)
	{
		//Initial rotation for vertical sides
		InitialRotation -= 90.f;
	}

	TArray<AChair*> &CurrChairs = ChairsOnAxis[FlipAxis].Chairs; //Get reference of array, in order to keep TMap updated
	const int32 ChairsCount = CurrChairs.Num();

	if ((ChairsCount * .5f) < ChairsPerSide) //Adding
	{
		const int32 ChairsToAdd = ChairsPerSide - (ChairsCount * .5f);
		for (int32 k = 0; k < ChairsToAdd; k++)
		{
			SpawnChairs(FlipAxis);
		}
	}
	else if (ChairsCount * .5f > ChairsPerSide && ChairsCount >= 2) //Deleting
	{
		CurrChairs[0]->Destroy();
		CurrChairs[1]->Destroy();
		CurrChairs.RemoveAt(0, 2);
	}

	int32 i = 0;
	while (i < CurrChairs.Num() - 1) //Updating
	{
		AChair* ChairToUpdate = CurrChairs[i++];
		FixChairTransform(*ChairToUpdate, StartSpawnPoint, SpawnOffset, InitialRotation, EAxes::None);

		ChairToUpdate = CurrChairs[i++];
		FixChairTransform(*ChairToUpdate, StartSpawnPoint, SpawnOffset, InitialRotation + 180.f, FlipAxis);

		//Updating spawn point position
		//If flip Axis is Y, then you're updating chairs on X Axis
		if (FlipAxis == EAxes::Y)
		{
			StartSpawnPoint.X -= TotalChairLength;
		}
		else if (FlipAxis == EAxes::X)
		{
			StartSpawnPoint.Y -= TotalChairLength;
		}
	}
}

void ATable::SpawnChairs(const EAxes FlipAxis)
{
	if (!ChairClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ChairClass is NULL. Unable to spawn the chair!"));
		return;
	}

	AChair* ChairSpawned = GetWorld()->SpawnActor<AChair>(ChairClass);
	if (!ChairSpawned)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn the chair!"));
		return;
	}

	ChairsOnAxis[FlipAxis].Chairs.Add(ChairSpawned);
	//ChairSpawned = nullptr;

	ChairSpawned = GetWorld()->SpawnActor<AChair>(ChairClass);
	ChairsOnAxis[FlipAxis].Chairs.Add(ChairSpawned);

}

void ATable::FixChairTransform(AChair &Chair, FVector StartSpawnPoint, FVector SpawnOffset, float Yaw, const EAxes FlipAxis) const
{
	if (FlipAxis == EAxes::Y)
	{
		SpawnOffset.Y *= -1;
	}
	if (FlipAxis == EAxes::X)
	{
		SpawnOffset.X *= -1;
	}

	//Set Transform
	const FVector NewLocation = StartSpawnPoint + SpawnOffset;
	Chair.SetActorLocation(NewLocation);
	Chair.SetActorRotation(FRotator(0, Yaw, 0));
}

void ATable::StartRecordingMovement()
{
	FHitResult HitResult;
	bool bHasHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (bHasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Recording"));

		const AActor* ActorHit = HitResult.GetActor();
		bool bIsResizePoint = ActorHit->IsA(AResizePoint::StaticClass());

		//Is the hit Resize Point attached to this table actor? Just for manage multiple instances of table
		bool bIsVaild = ActorHit->IsAttachedTo(this);

		if (bIsResizePoint && bIsVaild)
		{
			bRecordingMovement = bIsResizePoint;

			StartHitPoint = HitResult.ImpactPoint;
			StartHitPoint.Z = ActorHit->GetActorLocation().Z; //Just for more accurate calculus

			//Save current Vertices due to avoid exponential movement
			StartVertices = Vertices;
			StartCenter = Mesh->Bounds.GetBox().GetCenter();

			//FPlane plane = FPlane(StartHitPoint, FVector(0, 0, 1));
			//DrawDebugSolidPlane(GetWorld(), plane, StartHitPoint, FVector2D(100, 100), FColor::Red, true);
		}
	}
}

void ATable::StopRecordingMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Recording"));

	bRecordingMovement = false;
	StartHitPoint = FVector::ZeroVector;
	StartCenter = FVector::ZeroVector;
	StartVertices.Empty();
}

