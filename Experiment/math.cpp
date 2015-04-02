#include <iostream>
#include "lapacke.h"
#include <cblas.h>
#include <iterator>
#include <array>
#include <algorithm>
#include <string.h>

using namespace std;

// Zeros an nxn matrix
void zeros(double *A, int n) {
   double zero = 0.0;
   memset_pattern8(A, &zero, n*n*sizeof(*A));
}

// Sets all values of an nxn matrix to 1.0
void ones(double *A, int n) {
   double one = 1.0;
   memset_pattern8(A, &one, n*n*sizeof(*A));
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

inline void firstHalfStep(
   int n,
   double *Hx,
   double *Ux,
   double *Vx,
   double *Hy,
   double *Uy,
   double *Vy
) {

   // Original matlab source:
   //    Hx(i,j) = (H(i+1,j+1)+H(i,j+1))/2 - dt/(2*dx)*(U(i+1,j+1)-U(i,j+1));
   // Break that down into:
   //    <1.a> Hx(i,j) = (H(i+1,j+1)+H(i,j+1))/2)
   //    <1.b> Hx(i,j) = Hx(i,j) - dt/(2*dx)*(U(i+1,j+1)-U(i,j+1));
   // Break <1.a> into
   //    <1.a.1> Hx(i,j) = H(i+1,j+1)+H(i,j+1)
   //    <1.a.2> Hx(i,j) = Hx(i,j)/2
   // And since i = 1:n+1 and j = 1:n, 
   //    <1.a.1> Hx([1:n+1],[1:n]) = H([2:n+2],[2:n+1])+H([1:n+1],[2:n+1])
   //    <1.a.2> Hx([1:n+1],[1:n]) = Hx(i,j)/2

   // Height
   //    <1.a.1> Hx([1:n+1],[1:n]) = H([2:n+2],[2:n+1])+H([1:n+1],[2:n+1])
   Hx[0] = 2;
   Hx[3] = 2;
   Hx[6] = 2;
   Hx[1] = .1;
   Hx[4] = .2;
   Hx[7] = .3;

   printmat(Hx, n+1);

   const double *x = Hx+1;
   double *y = Hx;

   const int intt = 1;
   const double aaa = 1;

   for (int i = 0; i < n; i++) {
      cblas_daxpy(
         intt,
         aaa,
         x,
         intt,
         y,
         intt
      );
      /*
       * void cblas_daxpy(const int N, const double alpha, const double *X,
       *                  const int incX, double *Y, const int incY);
       */
   }
   cout << endl;
   printmat(Hx, n+1);
}


int main(int argc, char** argv) {
   cout << "Hello wrold " <<endl;

   int n = 64;

   n = 2; // testing

   double *H = matalloc(n+2);
   ones(H, n+2);
   double *U = matalloc(n+2);
   zeros(U, n+2);
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

   firstHalfStep(n, Hx, Ux, Vx, Hy, Uy, Vy);


   return 0;
}
