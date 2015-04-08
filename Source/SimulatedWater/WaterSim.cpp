// Fill out your copyright notice in the Description page of Project Settings.

#include "SimulatedWater.h"
#include "WaterSim.h"



AWaterSim::AWaterSim(const class FObjectInitializer & foi) : Super(foi) {

}

AWaterSim::AWaterSim() {
	
}


// Called when the game starts or when spawned
void AWaterSim::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;


	WaterTargetCapture = CastChecked<UWaterCanvas2D>(UCanvasRenderTarget2D::CreateCanvasRenderTarget2D(this, UWaterCanvas2D::StaticClass(), 1024, 1024));


	if ( WaterTargetCapture == nullptr ) {
		FError::Throwf(TEXT("WaterTargetCapture is null!"));
		return;
	}


	DoCycle();
	
}

// Called every frame
void AWaterSim::Tick( float DeltaTime )
{
	
	if ( WaterTargetCapture == NULL ) {
		FError::Throwf(TEXT("WaterTargetCapture is null 888888!"));
	}
	
	Super::Tick( DeltaTime );
	//DoCycle();
}

void AWaterSim::DoCycle() {

	if ( WaterTargetCapture != nullptr ) {
		WaterTargetCapture->UpdateResource();
	}
}

void AWaterSim::BeginDestroy() {
	if ( WaterTargetCapture != nullptr && WaterTargetCapture->IsValidLowLevel() ) {
		WaterTargetCapture->ConditionalBeginDestroy();
	}
	WaterTargetCapture = nullptr;

	Super::BeginDestroy();
}