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

	FError::Throwf(TEXT("HELLO"));
	
	PrimaryActorTick.bCanEverTick = true;

	TArray<FColor> SrcData;
	TArray<FColor> SrcData2;
	for ( int i = 0; i < 1024 * 1024; i++ ) {
		SrcData.Push(FColor(255.0, 0.0, 0.0));
		SrcData2.Push(FColor(0.0, 0.0, 255.0));
	}


	WaterTarget = FImageUtils::CreateTexture2D
		(1024, 1024, SrcData, this, "WaterTarget", RF_Public | RF_Transactional, FCreateTexture2DParameters());

	WaterTarget->CompressionNone = true;
	WaterTarget->MipGenSettings = TMGS_NoMipmaps;
	WaterTarget->CompressionSettings = TC_Default;
	
	if ( WaterTarget == nullptr ) {
		FError::Throwf(TEXT("WaterTarget is null!"));
		return;
	}
	
	int x = WaterTarget->PlatformData->Mips.Num();

	UE_LOG(LogTemp, Log, TEXT("Log text %d"), x);
	//WaterTarget->LinkStreaming();


	uint8 *MipData = static_cast<uint8*>( WaterTarget->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE) );

	UE_LOG(LogTemp, Log, TEXT("Element size is %d bytes"), WaterTarget->PlatformData->Mips[0].BulkData.GetElementSize());

	UE_LOG(LogTemp, Log, TEXT("The mip is %d by %d"), WaterTarget->PlatformData->Mips[0].SizeX, WaterTarget->PlatformData->Mips[0].SizeY);
	UE_LOG(LogTemp, Log, TEXT("I see <%d, %d, %d, %d>"), MipData[0], MipData[1], MipData[2], MipData[3]);

	FColor foo(0, 255, 0, 255);

	MipData[0] = foo.B;
	MipData[1] = foo.G;
	MipData[2] = foo.R;
	MipData[3] = foo.A;
	MipData[4] = foo.B;
	MipData[5] = foo.G;
	MipData[6] = foo.R;
	MipData[7] = foo.A;
	MipData[8] = foo.B;
	MipData[9] = foo.G;
	MipData[10] = foo.R;
	MipData[11] = foo.A; 
	
	UE_LOG(LogTemp, Log, TEXT("Now I see <%d, %d, %d, %d>"), MipData[0], MipData[1], MipData[2], MipData[3]);


	WaterTarget->PlatformData->Mips[0].BulkData.Unlock();
	
	WaterTarget->UpdateResource();
	//WaterTarget->InvalidateLastRenderTimeForStreaming();

	DoCycle();
	
}

// Called every frame
void AWaterSim::Tick( float DeltaTime )
{
	
	if ( WaterTarget == NULL ) {
		FError::Throwf(TEXT("WaterTarget is null 888888!"));
	}
	else {
		FError::Throwf(TEXT("WaterTarget is NOT NULL 888!"));
	}

	
	Super::Tick( DeltaTime );
	//DoCycle();
}

void AWaterSim::DoCycle() {

	
}

void AWaterSim::BeginDestroy() {
	if ( WaterTarget != nullptr && WaterTarget->IsValidLowLevel() ) {
		WaterTarget->ConditionalBeginDestroy();
		WaterTarget = nullptr;
	}

	Super::BeginDestroy();
}