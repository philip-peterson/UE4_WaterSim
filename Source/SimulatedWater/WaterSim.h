// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#define LAPACK_COMPLEX_STRUCTURE 
#include "lapacke_config.h"
#include "lapacke.h"

#include "GameFramework/Actor.h"
#include "ImageUtils.h"
#include "Engine/Texture2D.h"
#include "WaterSim.generated.h"

UCLASS()
class SIMULATEDWATER_API AWaterSim : public AActor
{
	GENERATED_UCLASS_BODY()
	
public:	

	AWaterSim();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly)
	UTexture2D *WaterTarget;

	virtual void BeginDestroy() override;
	
	void DoCycle();

};
