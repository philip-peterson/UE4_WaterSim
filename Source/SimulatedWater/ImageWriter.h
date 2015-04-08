// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ImageWriter.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATEDWATER_API UImageWriter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
		UFUNCTION(BlueprintCallable, Category="Utilities/ImageWriter")
		static bool WriteCanvasToCsv(UCanvasRenderTarget2D * canvas);

	
	
};
