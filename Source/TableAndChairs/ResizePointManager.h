// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ResizePointManager.generated.h"

class UResizePoint;

DECLARE_DELEGATE_OneParam(FResizePointMovedDelegate, const FVector&)

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TABLEANDCHAIRS_API UResizePointManager : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResizePointManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializePoints(const FVector &Center, const FVector &Extent);

	FResizePointMovedDelegate OnResizePointMovedDelegate;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<UResizePoint*> ResizePoints;

	/**  */
	APlayerController* PlayerController;


	/** Binds the functions for the InputComponent of the PlayerController  */
	void SetupInputBinding();

	void StartRecordingMovement();
	void StopRecordingMovement();

	FVector StartHitPoint;
	UResizePoint* ResizePointHit;
	FVector StartResizePointPosition;

	bool bRecordingMovement;

	UFUNCTION()
		void OnResizePointMoved(const FVector &ResizePointPosition);

};
