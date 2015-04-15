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


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D *MyCapture = nullptr;
	
	virtual void BeginDestroy() override;
	
	void DoCycle();

private:


	FSimulatedWaterWorker *Worker = nullptr;

};
