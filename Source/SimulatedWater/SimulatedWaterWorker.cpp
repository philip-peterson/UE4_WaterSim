// Fill out your copyright notice in the Description page of Project Settings.

#include "SimulatedWater.h"
#include "SimulatedWaterWorker.h"

bool FSimulatedWaterWorker::Init() {
	return true;
}

FORCEINLINE void FSimulatedWaterWorker::firstHalfStep(
	int iter,
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

	) {
	

		// x direction
		for ( int j = 0; j < n; j++ ) {
			for ( int i = 0; i < n + 1; i++ ) {

				// height
				Hx[i*( n + 1 ) + j] =
					( H[( i + 1 )*( n + 2 ) + j + 1] + H[i*( n + 2 ) + j + 1] ) / 2
					- dt / ( 2 * dx )*( U[( i + 1 )*( n + 2 ) + j + 1] - U[i*( n + 2 ) + j + 1] );


				// x momentum
				Ux[i*( n + 1 ) + j] =
					( U[( i + 1 )*( n + 2 ) + j + 1] + U[i*( n + 2 ) + j + 1] ) / 2 -
					dt / ( 2 * dx )*(
					(
					sq(U[( i + 1 )*( n + 2 ) + j + 1]) / H[( i + 1 )*( n + 2 ) + j + 1]
					+ g / 2 * sq(H[( i + 1 )*( n + 2 ) + j + 1])
					) - (
					sq(U[i*( n + 2 ) + j + 1]) / H[i*( n + 2 ) + j + 1] + g / 2 * sq(H[i*( n + 2 ) + j + 1])
					)
					);

				//// y momentum
				Vx[i*( n + 1 ) + j] = ( V[(i + 1) * (n+2) + j + 1] + V[i * (n+2) + j + 1] ) / 2 -
					dt / ( 2 * dx )*( ( U[( n + 2 )*( i + 1 ) + j + 1] * V[( i + 1 )*( n + 2 ) + j + 1] / H[( i + 1 )*( n + 2 ) + j + 1] ) -
					( U[( n + 2 )*(i)+j + 1] * V[i*( n + 2 ) + j + 1] / H[i*( n + 2 ) + j + 1] ) );
			}
		}

		// y direction
		for ( int j = 0; j < n + 1; j++ ) {
			for ( int i = 0; i < n; i++ ) {
				// height
				Hy[i*( n + 1 ) + j] = ( H[(i + 1)*(n+2) + j + 1] + H[(i + 1) * (n+2) + j] ) / 2 - dt / ( 2 * dy )*( V[(i + 1)*(n+2) + j + 1] - V[(i + 1) * (n+2) + j] );

				// x momentum
				Uy[i*( n + 1 ) + j] = ( U[( i + 1 )*( n + 2 ) + j + 1] + U[( i + 1 )*( n + 2 ) + j] ) / 2 -
					dt / ( 2 * dy )*( ( V[( i + 1 )*( n + 2 ) + j + 1] * U[( i + 1 )*( n + 2 ) + j + 1] / H[( i + 1 )*( n + 2 ) + j + 1] ) -
					( V[( i + 1 )*( n + 2 ) + j] * U[( i + 1 )*( n + 2 ) + j] / H[( i + 1 )*( n + 2 ) + j] ) );
				// y momentum
				Vy[i*(n+1) + j] = ( V[( i + 1 )*( n + 2 ) + j + 1] + V[( i + 1 )*( n + 2 ) + j] ) / 2 -
					dt / ( 2 * dy )*( ( sq(V[( i + 1 )*( n + 2 ) + j + 1]) / H[( i + 1 )*( n + 2 ) + j + 1] + g / 2 * sq(H[( n + 2 )*( i + 1 ) + j + 1]) ) -
					( sq(V[( i + 1 )*( n + 2 ) + j]) / H[( i + 1 )*( n + 2 ) + j] + g / 2 * sq(H[( i + 1 )*( n + 2 ) + j]) ) );
			}
		}

}

FORCEINLINE void FSimulatedWaterWorker::secondHalfStep(
	int iter,
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
	) {
		double temp;
		for ( int j = 1; j < n + 1; j++ ) {
			for ( int i = 1; i < n + 1; i++ ) {
				// height

				H[i * ( n + 2 ) + j] =  H[i * ( n + 2 ) + j] - ( dt / dx )*( Ux[i*( n + 1 ) + j - 1] - Ux[( i - 1 )*( n + 1 ) + j - 1] ) -
					( dt / dy )*( Vy[( i - 1 )*( n + 1 ) + j] - Vy[( i - 1 )*( n + 1 ) + j - 1] );
				// x momentum
				U[i * ( n + 2 ) + j] = U[i * ( n + 2 ) + j] - ( dt / dx )*( ( sq(Ux[i*( n + 1 ) + j - 1]) / Hx[i*( n + 1 ) + j - 1] + g / 2 * sq(Hx[i*( n + 1 ) + j - 1]) ) -
					( sq(Ux[( i - 1 )*( n + 1 ) + j - 1]) / Hx[( i - 1 )*( n + 1 ) + j - 1] + g / 2 * sq(Hx[( i - 1 )*( n + 1 ) + j - 1]) ) )
					- ( dt / dy )*( ( Vy[( i - 1 )*( n + 1 ) + j] * Uy[( i - 1 )*( n + 1 ) + j] / Hy[( i - 1 )*( n + 1 ) + j] ) -
					( Vy[( i - 1 )*( n + 1 ) + j - 1] * Uy[( i - 1 )*( n + 1 ) + j - 1] / Hy[( i - 1 )*( n + 1 ) + j - 1] ) );
				// y momentum
				temp = V[i * ( n + 2 ) + j] - ( dt / dx )*( ( Ux[i*( n + 1 ) + j - 1] * Vx[i*( n + 1 ) + j - 1] / Hx[i*( n + 1 ) + j - 1] ) -
					( Ux[( i - 1 )*( n + 1 ) + j - 1] * Vx[( i - 1 )*( n + 1 ) + j - 1] / Hx[( i - 1 )*( n + 1 ) + j - 1] ) )
					- ( dt / dy )*( ( sq(Vy[( i - 1 )*( n + 1 ) + j]) / Hy[( i - 1 )*( n + 1 ) + j] + g / 2 * sq(Hy[( i - 1 )*( n + 1 ) + j]) ) -
					( sq(Vy[( i - 1 )*( n + 1 ) + j - 1]) / Hy[( i - 1 )*( n + 1 ) + j - 1] + g / 2 * sq(Hy[( i - 1 )*( n + 1 ) + j - 1]) ) );


				V[i * ( n + 2 ) + j] = temp;
			}
		}
}

FORCEINLINE double FSimulatedWaterWorker::sq(double x) {
	return x*x;
}

// Zeros an nxn matrix
void FSimulatedWaterWorker::zeros(double *A, int n) {
	size_t nn = n*n;
	for ( size_t i = 0; i < nn; i++ ) {
		A[i] = 0.0;
	}
}

// Sets all values of an nxn matrix to 1.0
void FSimulatedWaterWorker::ones(double *A, int n) {
	size_t nn = n*n;
	for ( size_t i = 0; i < nn; i++ ) {
		A[i] = 1.0;
	}
}

double* FSimulatedWaterWorker::matalloc(int n) {
	double *mem = (double*)malloc(sizeof(*mem) * n * n);
	return mem;
}

void FSimulatedWaterWorker::Perturb(int cx, int cy, int radius) {
	int r2 = radius*radius;

	int minx = cx - radius;
	int miny = cy - radius;
	int maxx = cx + radius;
	int maxy = cy + radius;

	minx = FMath::Max(minx, 0);
	miny = FMath::Max(miny, 0);

	maxx = FMath::Min(maxx, n+1);
	maxy = FMath::Min(maxy, n + 1);

	for ( int i = minx; i <= maxx; i++ ) {
		for ( int j = miny; j <= maxy; j++ ) {
			int w = ( i - cx );
			int h = ( j - cy );
			if ( w*w + h*h <= r2 ) {
				V[i*( n + 2 ) + j] = 3.0;
			}
		}
	}
}

uint32 FSimulatedWaterWorker::Run() {

	int iter = 0;

	while ( ! bStopTask ) {

		if ( iter % 100 == 0 )
			Perturb(FMath::RandRange(0, n + 1), FMath::RandRange(0, n + 1), 5);


		// Reflective boundary conditions

		for ( int i = 0; i < n + 2; i++ ) {
			H[i*( n + 2 ) + 0] = H[i*( n + 2 ) + 1];
			U[i*( n + 2 ) + 0] = U[i*( n + 2 ) + 1];
			V[i*( n + 2 ) + 0] = -V[i*( n + 2 ) + 1];

			H[i*( n + 2 ) + n + 1] = H[i*( n + 2 ) + n];
			U[i*( n + 2 ) + n + 1] = U[i*( n + 2 ) + n];
			V[i*( n + 2 ) + n + 1] = -V[i*( n + 2 ) + n];

			H[0 * ( n + 2 ) + i] = H[1 * ( n + 2 ) + i];
			U[0 * ( n + 2 ) + i] = -U[1 * ( n + 2 ) + i];
			V[0 * ( n + 2 ) + i] = V[1 * ( n + 2 ) + i];

			H[( n + 1 ) * ( n + 2 ) + i] = H[( n + 0) * ( n + 2 ) + i];
			U[( n + 1 ) * ( n + 2 ) + i] = -U[( n + 0 ) * ( n + 2 ) + i];
			V[( n + 1 ) * ( n + 2 ) + i] = V[( n + 0 ) * ( n + 2 ) + i];
		}


		firstHalfStep(iter, n, 9.8, dt, dx, dy, H, U, V, Hx, Ux, Vx, Hy, Uy, Vy);
		secondHalfStep(iter, n, 9.8, dt, dx, dy, H, U, V, Hx, Ux, Vx, Hy, Uy, Vy);


		{
			FScopeLock Lock(&AccessPublicBuffer);
			for ( int i = 0; i < n+2; i++ ) {
				for ( int j = 0; j < n+2; j++ ) {
					PublicBuffer[i*(n+2)+j] = FMath::Lerp<uint8, float>(0, 255, FMath::Clamp<double>(H[i*(n+2)+j]+1, 0, 4)/4.0);
				}
			}
		}

		iter++;

	}


	return 0;
}

void FSimulatedWaterWorker::Stop() {
	bStopTask = true;
}

void FSimulatedWaterWorker::Shutdown() {
	Stop();
	Thread->WaitForCompletion();
	free(H);
	free(U);
	free(V);
	free(Hx);
	free(Ux);
	free(Vx);
	free(Hy);
	free(Uy);
	free(Vy);
	free(PublicBuffer);
}

void FSimulatedWaterWorker::Exit() {

}

/*
 * Creates a Simulated Water worker with a texture buffer size of n by n.
 * Returns null if the platform does not support multithreading or n is too small.
 */
FSimulatedWaterWorker* FSimulatedWaterWorker::Create(uint16 n, double dt, double ds, double DampingFactor, int32 DampingInfrequency) {
	if ( !FPlatformProcess::SupportsMultithreading() || n < 3 ) {
		return nullptr;
	}
	FSimulatedWaterWorker *x = new FSimulatedWaterWorker(n-2, dt, ds, DampingFactor, DampingInfrequency);
	return x;
}

FSimulatedWaterWorker::FSimulatedWaterWorker(uint16 n, double dt, double ds, double DampingFactor, int32 DampingInfrequency)
	: 
		n(n),
		bStopTask(false),
		dt(dt),
		dx(ds),
		dy(ds),
		DampingFactor(DampingFactor),
		DampingInfrequency(DampingInfrequency)
	{

	H = matalloc(n + 2);
	ones(H, n + 2);

	U = matalloc(n + 2);
	zeros(U, n + 2);

	V = matalloc(n + 2);
	zeros(V, n + 2);

	Hx = matalloc(n + 1);
	zeros(Hx, n + 1);

	Ux = matalloc(n + 1);
	zeros(Ux, n + 1);

	Vx = matalloc(n + 1);
	zeros(Vx, n + 1);

	Hy = matalloc(n + 1);
	zeros(Hy, n + 1);

	Uy = matalloc(n + 1);
	zeros(Uy, n + 1);

	Vy = matalloc(n + 1);
	zeros(Vy, n + 1);


	

	PublicBuffer = (uint8*)malloc(( n + 2 )*( n + 2 )*sizeof(*PublicBuffer));

	Thread = FRunnableThread::Create(this, TEXT("FSimulatedWaterWorker"), 0, TPri_BelowNormal);

}

FSimulatedWaterWorker::~FSimulatedWaterWorker() {
	delete Thread;
	Thread = nullptr;
}
