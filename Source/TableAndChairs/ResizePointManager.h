// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ResizePointManager.generated.h"

class UResizePoint;

DECLARE_DELEGATE_ThreeParams(FResizePointMovedDelegate, const bool, const UResizePoint*, const FVector&)

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

	void InitializePoints(const FVector &ParentExtent, USceneComponent* ParentComp);

	FResizePointMovedDelegate OnResizePointMovedDelegate;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<UResizePoint*> ResizePoints;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMesh* ResizePointMesh;

	/**  */
	APlayerController* PlayerController;

	/** Binds the functions for the InputComponent of the PlayerController  */
	void SetupInputBinding();

	/** Casts a Raycast, if it hits a ResizePoint it sets the variables needed to calculate the movement. */
	void StartRecordingMovement();

	/** Stops the mouse tracking and resets the start variables. */
	void StopRecordingMovement();

	FVector StartHitPoint;
	UResizePoint* ResizePointHit;

	/** Are you actually resizing the mesh? */
	bool bRecordingMovement;

	/** Calculats the delta movement of the ResizePoint and resizes the object by that amount
	* @param IsValid - Is the CheckedPosition valid?
	* @param ResizePointRef - A reference to the Resize Point that has just been moved
	* @param CheckedPosition - The new position of the Resize Point
	*/
	UFUNCTION()
		void OnPositionChecked(const bool IsValid, const UResizePoint *ResizePointRef, const FVector &CheckedPosition);
};
