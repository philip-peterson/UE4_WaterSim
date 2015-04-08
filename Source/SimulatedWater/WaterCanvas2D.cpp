// Fill out your copyright notice in the Description page of Project Settings.

#include "SimulatedWater.h"
#include "WaterCanvas2D.h"



#include <time.h>

void UWaterCanvas2D::ReceiveUpdate(UCanvas* Canvas, int32 Width, int32 Height) {
	/*
	int32 s;
	float ps;
	int32 s2;
	float ps2;

	UGameplayStatics::GetAccurateRealTime(this, s, ps);

	for ( int32 y = 1; y <= Height; y++ ) {
		for ( int32 x = 1; x <= Width; x++ ) {
			Canvas->K2_DrawLine(FVector2D(x, y), FVector2D(x + 1, y), 1.0f, FLinearColor(x / 1024.0, y / 1024.0, y / 1024.0));
		}
	}
	UGameplayStatics::GetAccurateRealTime(this, s2, ps2);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Time: %f"),  (s2+ps2/1000.0)-(s+ps/1000.0)   ));
	*/
}