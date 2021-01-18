// Fill out your copyright notice in the Description page of Project Settings.


#include "Table.h"
#include "Chair.h"
#include "TableLeg.h"
#include "ResizePoint.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
//#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATable::ATable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VertexCount = (6 * 4); //6 faces with 4 vertices each
	Vertices.AddUninitialized(VertexCount);

	TrianglesCount = (6 * 2 * 3); //2 triangles per face, 3 vertices each
	Triangles.AddUninitialized(TrianglesCount);

	Width = 200;
	Length = 200;
	Height = 20;

	bRecordingMovement = false;

	ChairsOnAxis.Add(EAxes::X, new TArray<AChair*>());
	ChairsOnAxis.Add(EAxes::Y, new TArray<AChair*>());

	ChairOffset = 15.f;
	ChairWidth = 60.f + (ChairOffset * 2);
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

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
	for (int32 i = 0; i < LegsCount; i++)
	{
		ATableLeg* Leg = GetWorld()->SpawnActor<ATableLeg>(ATableLeg::StaticClass());

		if (Leg)
		{
			Leg->GenerateMesh(TArray<FLinearColor>());
			Legs.Add(Leg);
		}
	}
}

void ATable::SpawnResizePoints()
{
	//Generate ResizePoints
	for (int32 i = 0; i < ResizePointsCount; i++)
	{
		AResizePoint* ResizePointInstance = GetWorld()->SpawnActor<AResizePoint>(AResizePoint::StaticClass());
		ResizePointInstance->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		ResizePoints.Add(ResizePointInstance);
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

	//6 sides on cube, 4 verts each (corners)

	const FVector Scale = FVector(Width, Length, Height);
	const float LegHeight = GetTableLegSize().Z;
	const FVector Position = FVector(0, 0, LegHeight + Height);

	BuildCube(FVector(0, 0, 0), FVector(0, 1, 0), FVector(0, 1, 1), FVector(0, 0, 1), FVector(1, 1, 0), FVector(1, 0, 0), FVector(1, 0, 1), FVector(1, 1, 1));

	//FTransform* TableTransform = new FTransform(FRotator::ZeroRotator, Position, Scale);
	//FMatrix TableMatrix = TableTransform->ToMatrixWithScale();

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		//Vertices[i] = TableMatrix.TransformPosition(Vertices[i]);
		Vertices[i] *= Scale;
	}

	TArray<FLinearColor> VertexColors;
	//VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	//VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	GenerateMesh(VertexColors);
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
	Mesh->UpdateMeshSection_LinearColor(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
}


void ATable::UpdateLegsTransform()
{
	if (Legs.Num() == 0)
	{
		return;
	}

	const ATableLeg* Leg = Legs[0];
	const FBox TableBounds = Mesh->Bounds.GetBox();

	TArray<FVector> LegsPositions = {
		FVector(TableBounds.Min.X, TableBounds.Min.Y, TableBounds.Min.Z - Leg->Height),
		FVector(TableBounds.Min.X, TableBounds.Max.Y - Leg->Length, TableBounds.Min.Z - Leg->Height),
		FVector(TableBounds.Max.X - Leg->Width, TableBounds.Min.Y, TableBounds.Min.Z - Leg->Height),
		FVector(TableBounds.Max.X - Leg->Width, TableBounds.Max.Y - Leg->Length, TableBounds.Min.Z - Leg->Height)
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

FVector ATable::GetTableLegSize() const
{
	if (Legs.Num() == 0)
	{
		return FVector::ZeroVector;
	}

	const ATableLeg* Leg = Legs[0];
	return Leg->Mesh->Bounds.GetBox().GetSize();
}

void ATable::CalculateChairs()
{
	const FBox Bounds = Mesh->Bounds.GetBox();
	const FVector TableSize = Bounds.GetSize();

	const FVector TableLegSize = GetTableLegSize();

	const int32 ChairsPerSideX = FMath::FloorToInt((TableSize.X - 60.f) / ChairWidth);
	const int32 ChairsPerSideY = FMath::FloorToInt((TableSize.Y - 60.f) / ChairWidth);

	//VERTICAL SIDES - Top and Bottom - Need to flip X-Axis
	float TotalChairLength = GetTotalChairLength(ChairsPerSideY, TableSize.Y - 60.f);

	FVector StartSpawnPoint = FVector(Bounds.GetCenter().X - DistanceFromTable, Bounds.Max.Y - TableLegSize.Y, Bounds.Min.Z); // Starting from right-center of table
	FVector SpawnOffset = FVector((TableSize.X  * .5f) + DistanceFromTable, (-TotalChairLength * .5f), 0);

	CalculateChairsOfAxis(StartSpawnPoint, SpawnOffset, ChairsPerSideY, TotalChairLength, EAxes::X);

	//HORIZONTAL SIDES - Right and Left - Need to flip Y-Axis
	TotalChairLength = GetTotalChairLength(ChairsPerSideX, TableSize.X - 60.f);

	StartSpawnPoint = FVector(Bounds.Max.X - TableLegSize.X, Bounds.GetCenter().Y - DistanceFromTable, Bounds.Min.Z); // Starting from top-center of table
	SpawnOffset = FVector((-TotalChairLength * .5f), (TableSize.Y  * .5f) + DistanceFromTable, 0);

	CalculateChairsOfAxis(StartSpawnPoint, SpawnOffset, ChairsPerSideX, TotalChairLength, EAxes::Y);
}

float ATable::GetTotalChairLength(const int ChairsPerSide, const float TableSideLength) const
{
	if (ChairsPerSide == 0) //Can't divide by 0
	{
		return 0;
	}

	float TotalChairsLength = ChairsPerSide * ChairWidth;
	float AdditionalChairOffset = (TableSideLength - TotalChairsLength) / ChairsPerSide;

	return ChairWidth + AdditionalChairOffset;
}

void ATable::CalculateChairsOfAxis(FVector StartSpawnPoint, FVector SpawnOffset, const int ChairsPerSide, const float TotalChairLength, const EAxes FlipAxis)
{
	float InitialRotation = Mesh->GetRelativeRotation().Yaw;

	if (FlipAxis == EAxes::X)
	{
		InitialRotation -= 90.f;
	}

	TArray<AChair*> &CurrChairs = *ChairsOnAxis[FlipAxis];
	const int32 ChairsCount = CurrChairs.Num();

	if (ChairsCount * .5f < ChairsPerSide) //Adding
	{
		const int32 ChairsToAdd = ChairsPerSide - (ChairsCount * .5f);
		for (int32 k = 0; k < ChairsToAdd; k++)
		{
			GenerateChairsFromPoint(StartSpawnPoint, InitialRotation, FlipAxis);
		}
	}
	else if (ChairsCount * .5f > ChairsPerSide) //Deleting
	{
		CurrChairs[0]->Destroy();
		CurrChairs[1]->Destroy();
		CurrChairs.RemoveAt(0, 2);
	}

	int32 i = 0;
	while (i < CurrChairs.Num() - 1) //Updating
	{
		AChair* ChairToUpdate = CurrChairs[i++];
		FixCharirTransform(*ChairToUpdate, StartSpawnPoint, SpawnOffset, InitialRotation, EAxes::None);

		ChairToUpdate = CurrChairs[i++];
		FixCharirTransform(*ChairToUpdate, StartSpawnPoint, SpawnOffset, InitialRotation + 180.f, FlipAxis);

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

void ATable::GenerateChairsFromPoint(const FVector &StartSpawnPoint, float InitialRotation, const EAxes FlipAxis)
{
	AChair* ChairSpawned = GetWorld()->SpawnActor<AChair>(AChair::StaticClass());
	if (!ChairSpawned)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn chair!"));
		return;
	}

	//FixCharirTransform(*ChairSpawned, StartSpawnPoint, InitialRotation, EAxes::None);
	ChairsOnAxis[FlipAxis]->Add(ChairSpawned);
	ChairSpawned = nullptr;

	ChairSpawned = GetWorld()->SpawnActor<AChair>(AChair::StaticClass());
	//FixCharirTransform(*ChairSpawned, StartSpawnPoint, InitialRotation + 180.f, FlipAxis);
	ChairsOnAxis[FlipAxis]->Add(ChairSpawned);

}

void ATable::FixCharirTransform(AChair &Chair, FVector StartSpawnPoint, FVector SpawnOffset, float Yaw, const EAxes FlipAxis)
{
	if (FlipAxis == EAxes::Y)
	{
		SpawnOffset.Y *= -1;
	}
	if (FlipAxis == EAxes::X)
	{
		SpawnOffset.X *= -1;
	}

	const FVector NewLocation = StartSpawnPoint + SpawnOffset;
	Chair.SetActorLocation(NewLocation);
	Chair.SetActorRotation(FRotator(0, Yaw, 0));
}

/**
 * Casts a Raycast, if it hits a ResizePoint it sets the variables needed to calculate the movement.
 */
void ATable::StartRecordingMovement()
{
	FHitResult HitResult;
	bool bHasHit = PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	if (bHasHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Recording"));

		const AActor* ActorHit = HitResult.GetActor();
		bool bIsResizePoint = ActorHit->IsA(AResizePoint::StaticClass());

		if (bIsResizePoint)
		{
			bRecordingMovement = bIsResizePoint;

			StartHitPoint = HitResult.ImpactPoint;
			StartHitPoint.Z = ActorHit->GetActorLocation().Z; //Just for more accurate calculus

			StartVertices = Vertices;
			StartCenter = Mesh->Bounds.GetBox().GetCenter();

			//FPlane plane = FPlane(StartHitPoint, FVector(0, 0, 1));
			//DrawDebugSolidPlane(GetWorld(), plane, StartHitPoint, FVector2D(100, 100), FColor::Red, true);
		}

	}
}

/**
 * Stops the mouse tracking and resets the start variables.
 */
void ATable::StopRecordingMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Recording"));

	bRecordingMovement = false;
	StartHitPoint = FVector::ZeroVector;
	StartCenter = FVector::ZeroVector;
	StartVertices.Empty();
}

