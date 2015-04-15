// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

class FSimulatedWaterWorker : public FRunnable {

	virtual bool Init() override;

	virtual uint32 Run() override;
	
	virtual void Stop() override;

	virtual void Exit() override;

private:
	FRunnableThread* Thread;

	FThreadSafeBool bStopTask;

	FORCEINLINE void firstHalfStep(
		int n,
		double g,
		double dt,
		double dx,
		double dy,

		double *H,
		double *U,
		double *V,
		double *Hx,
		double *Ux,
		double *Vx,
		double *Hy,
		double *Uy,
		double *Vy

	);

	FORCEINLINE void secondHalfStep(
		int n,
		double g,
		double dt,
		double dx,
		double dy,

		double *H,
		double *U,
		double *V,
		double *Hx,
		double *Ux,
		double *Vx,
		double *Hy,
		double *Uy,
		double *Vy
		);

	FORCEINLINE double sq(double x);

	void zeros(double *A, int n);
	void ones(double *A, int n);
	double *matalloc(int n);

	int n;
	double dt = 0.01;
	double dx = 1.0;
	double dy = 1.0;
	double *H;
	double *U;
	double *V;
	double *Hx;
	double *Ux;
	double *Vx;
	double *Hy;
	double *Uy;
	double *Vy;

	FSimulatedWaterWorker(int n);

public:

	FCriticalSection AccessPublicBuffer;

	uint8 *PublicBuffer;

	const float *LockBuffer();

	void UnlockBuffer();

	static FSimulatedWaterWorker *Create(int n);

	void Shutdown();

	virtual ~FSimulatedWaterWorker() override;
};