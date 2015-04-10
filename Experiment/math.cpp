#include <iostream>
#include <cblas.h>
#include <iterator>
#include <array>
#include <algorithm>

using namespace std;

// Zeros an nxn matrix
void zeros(double *A, int n) {
   size_t nn = n*n;
   for (size_t i = 0; i < nn; i++) {
      A[i] = 0.0;
   }
}

// Sets all values of an nxn matrix to 1.0
void ones(double *A, int n) {
   size_t nn = n*n;
   for (size_t i = 0; i < nn; i++) {
      A[i] = 1.0;
   }
}

double *matalloc(int n) {
   double *mem = (double*)malloc(sizeof(*mem) * n * n);
   return mem;
}

void printmat(double* H, int n) {
   for (int i = 0; i < n*n; i++) {
      cout << H[i];
      if (i != 1 && (i+1) % n == 0) {
         cout << endl;
      }
      else {
         cout << " ";
      }
   }
}

void printvec(double* H, int n) {
   for (int i = 0; i < n; i++) {
      cout << H[i];
      cout << endl;
   }
   cout << endl;
}

inline double sq(double a) {
   return a*a;
}

inline void firstHalfStep(
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
   for (int j = 0; j < n; j++) {
      for (int i = 0; i < n+1; i++) {
         
       // height
       Hx[i*(n+1)+j] = 
         (H[(i+1)*(n+2)+j+1]+H[i*(n+2)+j+1])/2
         - dt/(2*dx)*(U[(i+1)*(n+2)+j+1]-U[i*(n+2)+j+1]);
   
       // x momentum
       Ux[i*(n+1),j] = 
            (U[(i+1)*(n+2),j+1]+U[i*(n+2),j+1])/2 - 
            dt/(2*dx)*(
                  (
                     sq(U[(i+1)*(n+2)+j+1])/H[(i+1)*(n+2),j+1] 
                     + g/2*sq(H[(i+1)*(n+2)+j+1])
                  ) - (
                     sq(U[i*(n+2)+j+1])/H[i*(n+2)+j+1] + g/2*sq(H[i*(n+2)+j+1])
                  )
            );
   
       //// y momentum
       Vx[i,j] = (V[i+1,j+1]+V[i,j+1])/2 - 
                 dt/(2*dx)*((U[(n+2)*(i+1)+j+1]*V[(i+1)*(n+2)+j+1]/H[(i+1)*(n+2)+j+1]) -
                            (U[(n+2)*(i)+j+1]*V[i*(n+2)+j+1]/H[i*(n+2)+j+1]));
      }
   }

   // y direction
   for (int j = 0; j < n+1; j++) {
      for (int i = 0; i < n; i++) {
       // height
       Hy[i,j] = (H[i+1,j+1]+H[i+1,j])/2 - dt/(2*dy)*(V[i+1,j+1]-V[i+1,j]);
   
       // x momentum
       Uy[i,j] = (U[(i+1)*(n+2)+j+1]+U[(i+1)*(n+2)+j])/2 -
                 dt/(2*dy)*((V[(i+1)*(n+2)+j+1]*U[(i+1)*(n+2)+j+1]/H[(i+1)*(n+2)+j+1]) -
                            (V[(i+1)*(n+2)+j]*U[(i+1)*(n+2)+j]/H[(i+1)*(n+2)+j]));
       // y momentum
       Vy[i,j] = (V[(i+1)*(n+2)+j+1]+V[(i+1)*(n+2)+j])/2 -
                 dt/(2*dy)*((sq(V[(i+1)*(n+2)+j+1])/H[(i+1)*(n+2)+j+1] + g/2*sq(H[(n+2)*(i+1)+j+1])) -
                            (sq(V[(i+1)*(n+2)+j])/H[(i+1)*(n+2)+j] + g/2*sq(H[(i+1)*(n+2)+j])));
      }
   }
   cout << endl;
}

inline void secondHalfStep(
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
   for (int j = 1; j < n+1; j++) {
      for (int i = 1; i < n+1; i++) {
         // height

         H[i,j] = H[i,j] - (dt/dx)*(Ux[i*sizeofUX+j-1]-Ux[(i-1)*sizeofUX+j-1]) - ...
                           (dt/dy)*(Vy[(i-1)*sizeofVY+j]-Vy[(i-1)*sizeofVY+j-1]);
         // x momentum
         U[i,j] = U[i,j] - (dt/dx)*((sq(Ux[i*sizeofUX+j-1])/Hx[i*sizeofHX+j-1] + g/2*sq(Hx[i*sizeofHX+j-1])) -
                           (sq(Ux[(i-1)*sizeofUX+j-1])/Hx[(i-1)*sizeofHX+j-1] + g/2*sq(Hx[(i-1)*sizeofHX+j-1])))
                         - (dt/dy)*((Vy[(i-1)*sizeofVY+j]*Uy[(i-1)*sizeofUY+j]/Hy[(i-1)*sizeofHY+j]) - ...
                           (Vy[(i-1)*sizeofVY+j-1]*Uy[(i-1)*sizeofUY+j-1]/Hy[(i-1)*sizeofHY+j-1]));
         // y momentum
         V[i,j] = V[i,j] - (dt/dx)*((Ux[i*sizeofUX+j-1]*Vx[i*sizeofVX+j-1]/Hx[i*sizeofHX+j-1]) - ...
                           (Ux[(i-1)*sizeofUX+j-1]*Vx[i-1,j-1]/Hx[i-1,j-1])) ...
                         - (dt/dy)*((sq(Vy[i-1,j])/Hy[i-1,j] + g/2*sq(Hy[i-1,j])) - ...
                           (sq(Vy[i-1,j-1])/Hy[i-1,j-1] + g/2*sq(Hy[i-1,j-1])));
      }
   }
}


int main(int argc, char** argv) {
   int n = 64;
   double dt = 0.1;
   double dx = 1.0;
   double dy = 1.0;

   n = 2; // testing

   double *H = matalloc(n+2);
   ones(H, n+2);
   double *U = matalloc(n+2);
   zeros(U, n+2);
   U[10]=17;
   U[14]=1337;
   double *V = matalloc(n+2);
   zeros(V, n+2);

   double *Hx = matalloc(n+1);
   ones(Hx, n+1);
   double *Ux = matalloc(n+1);
   zeros(Ux, n+1);
   double *Vx = matalloc(n+1);
   zeros(Vx, n+1);

   double *Hy = matalloc(n+1);
   ones(Hy, n+1);
   double *Uy = matalloc(n+1);
   zeros(Uy, n+1);
   double *Vy = matalloc(n+1);
   zeros(Vy, n+1);

   double *swap = (double*)malloc(sizeof(*swap) * (n+1));

   firstHalfStep(n, -9.8, dt, dx, dy, H, U, V, Hx, Ux, Vx, Hy, Uy, Vy);
   secondHalfStep(n, -9.8, dt, dx, dy, H, U, V, Hx, Ux, Vx, Hy, Uy, Vy);

   return 0;
}
