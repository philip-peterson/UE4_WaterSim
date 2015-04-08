// Fill out your copyright notice in the Description page of Project Settings.

#include "SimulatedWater.h"
#include "WaterSim.h"
#include "Kismet/KismetMathLibrary.h"



AWaterSim::AWaterSim(const class FObjectInitializer & foi) : Super(foi) {

}

AWaterSim::AWaterSim() {
	
}


// Called when the game starts or when spawned
void AWaterSim::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;


	MyCapture = UTexture2D::CreateTransient(1024, 1024, PF_B8G8R8A8);

	DoCycle();
	
}

// Called every frame
void AWaterSim::Tick( float DeltaTime )
{

	Super::Tick(DeltaTime);
	
	if ( MyCapture == NULL ) {
		FError::Throwf(TEXT("MyCapture is null 888888!"));
	}
	else {
		DoCycle();
	}

}

void AWaterSim::DoCycle() {

		
	FTexture2DMipMap *mip = &( MyCapture->PlatformData->Mips[0] );
		
	uint8* MipData = (uint8*)mip->BulkData.Lock(LOCK_READ_WRITE);

	const int32 Width = mip->SizeX;
	const int32 Height = mip->SizeY;

	for ( int32 y=0; y<Height; y++ )
	{
		uint8* DestPtr = &MipData[( Height - 1 - y ) * Width * sizeof(FColor)];
		FColor SrcPtr(255,255,0,255);
		for ( int32 x=0; x<Width; x++ )
		{
			*DestPtr++ = SrcPtr.B;
			*DestPtr++ = SrcPtr.G;
			*DestPtr++ = SrcPtr.R;
			*DestPtr++ = SrcPtr.A;
		}
	}

	mip->BulkData.Unlock();

	// Set options
	MyCapture->SRGB = false;
	MyCapture->CompressionNone = true;
	MyCapture->MipGenSettings = TMGS_NoMipmaps;
	MyCapture->CompressionSettings = TC_Default; //TC_Default
	
	// Update the remote texture data
	MyCapture->UpdateResource();

	
}

void AWaterSim::BeginDestroy() {


	Super::BeginDestroy();
}