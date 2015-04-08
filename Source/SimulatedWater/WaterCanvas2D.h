// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/CanvasRenderTarget2D.h"
#include "WaterCanvas2D.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATEDWATER_API UWaterCanvas2D : public UCanvasRenderTarget2D
{
	GENERATED_BODY()

		virtual void ReceiveUpdate(UCanvas* Canvas, int32 Width, int32 Height) override;
	
	
};
