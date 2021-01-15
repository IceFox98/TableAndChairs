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

	ResizePointsCount = 4;
	bRecordingMovement = false;
}

// Called when the game starts or when spawned
void ATable::BeginPlay()
{
	Super::BeginPlay();

	BuildMesh();

	//Generate Table's Legs
	const int32 LegCount = 4;

	for (int32 i = 0; i < LegCount; i++)
	{
		ATableLeg* Leg = GetWorld()->SpawnActor<ATableLeg>(ATableLeg::StaticClass());

		if (Leg)
		{
			Leg->GenerateMesh(TArray<FLinearColor>());
			Legs.Add(Leg);
		}
	}

	//Generate ResizePoints
	for (int32 i = 0; i < ResizePointsCount; i++)
	{
		AResizePoint* ResizePointInstance = GetWorld()->SpawnActor<AResizePoint>(AResizePoint::StaticClass());
		ResizePointInstance->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		ResizePoints.Add(ResizePointInstance);
	}

	UpdateResizePointsTransform();
	UpdateLegsTransform();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController && PlayerController->InputComponent)
	{
		PlayerController->InputComponent->BindAction("RecordMovement", IE_Pressed, this, &ATable::StartRecordingMovement);
		PlayerController->InputComponent->BindAction("RecordMovement", IE_Released, this, &ATable::StopRecordingMovement);
	}
}


void ATable::BuildMesh()
{
	Super::BuildMesh();

	//6 sides on cube, 4 verts each (corners)

	FVector Scale = FVector(Width, Length, Height);
	FVector Position = FVector(0, 0, 100 + Height);

	BuildCube(FVector(0, 0, 0), FVector(0, 1, 0), FVector(0, 1, 1), FVector(0, 0, 1), FVector(1, 1, 0), FVector(1, 0, 0), FVector(1, 0, 1), FVector(1, 1, 1));

	FTransform* TableTransform = new FTransform(FRotator::ZeroRotator, Position, Scale);
	FMatrix TableMatrix = TableTransform->ToMatrixWithScale();

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		Vertices[i] = TableMatrix.TransformPosition(Vertices[i]);
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
	}
}

/**
 * Updates the Mesh of the table according to the point hit by mouse.
 * @param MovementAmount - The total movement tracked by the mouse.
 */
void ATable::UpdateTableMesh(const FVector &MovementAmount)
{
	//Get the sign to know the direction of the movement
	float xSign = FMath::Sign(StartHitPoint.X - StartCenter.X);
	float ySign = FMath::Sign(StartHitPoint.Y - StartCenter.Y);

	for (int32 i = 0; i < Vertices.Num(); i++)
	{
		FVector CurrMovementAmount = MovementAmount;
		FVector CurrVertex = StartVertices[i];

		//Freeze the vertex location 
		if (FMath::Sign(CurrVertex.X - StartCenter.X) != xSign)
		{
			CurrMovementAmount.X = 0;
		}

		if (FMath::Sign(CurrVertex.Y - StartCenter.Y) != ySign)
		{
			CurrMovementAmount.Y = 0;
		}

		Vertices[i] = CurrVertex + FVector(CurrMovementAmount.X, CurrMovementAmount.Y, 0);
	}

	//Updating Mesh
	Mesh->UpdateMeshSection_LinearColor(0, Vertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
}

/**
 * Updates the table's legs position according to table's bounds.
 */
void ATable::UpdateLegsTransform()
{
	if (Legs.Num() == 0)
	{
		return;
	}

	const ATableLeg* Leg = Legs[0];
	const FBox Bounds = Mesh->Bounds.GetBox();

	TArray<FVector> LegsPositions = {
		FVector(Bounds.Min.X, Bounds.Min.Y, Bounds.Min.Z - Leg->Height),
		FVector(Bounds.Min.X, Bounds.Max.Y - Leg->Length, Bounds.Min.Z - Leg->Height),
		FVector(Bounds.Max.X - Leg->Width, Bounds.Min.Y, Bounds.Min.Z - Leg->Height),
		FVector(Bounds.Max.X - Leg->Width, Bounds.Max.Y - Leg->Length, Bounds.Min.Z - Leg->Height)
	};

	UpdateTransforms((TArray<AActor*>)Legs, LegsPositions);

}

/**
 * Updates the resize points' position according to table's bounds.
 */
void ATable::UpdateResizePointsTransform()
{
	if (ResizePoints.Num() == 0)
	{
		return;
	}

	const FBox Bounds = Mesh->Bounds.GetBox();

	TArray<FVector> ResizePointsPositions = {
		FVector(Bounds.Min.X, Bounds.Min.Y, Bounds.Max.Z),
		FVector(Bounds.Min.X, Bounds.Max.Y, Bounds.Max.Z),
		FVector(Bounds.Max.X, Bounds.Min.Y, Bounds.Max.Z),
		FVector(Bounds.Max.X, Bounds.Max.Y, Bounds.Max.Z)
	};

	UpdateTransforms((TArray<AActor*>)ResizePoints, ResizePointsPositions);
}

void ATable::UpdateTransforms(TArray<AActor*> ActorsToUpdate, const TArray<FVector> &NewPositions)
{
	if (ActorsToUpdate.Num() == NewPositions.Num())
	{
		for (int i = 0; i < NewPositions.Num(); i++)
		{
			FVector NewPosition = NewPositions[i];
			ActorsToUpdate[i]->SetActorLocation(NewPosition);
		}
	}
}

void ATable::CalculateChairs()
{

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

