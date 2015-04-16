// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


/*
#define LAPACK_COMPLEX_STRUCTURE 
#include "lapacke_config.h"
#include "lapacke.h"
*/

#include "GameFramework/Actor.h"
#include "ImageUtils.h"
#include "Engine/Texture2D.h"
#include "WaterCanvas2D.h"
#include "SimulatedWaterWorker.h"
#include "WaterSim.generated.h"

UCLASS()
class SIMULATEDWATER_API AWaterSim : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DampingFactor = .01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 DampingInfrequency = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 GridSize = 256;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( FriendlyName = "Time Step" ))
		float dt = 0.01;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=( FriendlyName = "Space Step" ))
		float ds = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DesiredFramerate = 15;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D *MyCapture = nullptr;


	
	virtual void BeginDestroy() override;
	
	void DoCycle();

private:

	int n;

	float Timer;

	FSimulatedWaterWorker *Worker = nullptr;

};
