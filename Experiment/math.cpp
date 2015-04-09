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

inline void firstHalfStep(
   int n,
   double dt,
   double dx,
   double *H,
   double *Hx,
   double *Ux,
   double *Vx,
   double *Hy,
   double *Uy,
   double *Vy,
   double *swap
) {

   H[0]=.1;
   H[1]=.2;
   H[2]=.3;
   H[3]=.4;

   H[4]=.5;
   H[5]=.6;
   H[6]=.7;
   H[7]=.8;

   H[8]=.9;
   H[9]=.109;
   H[10]=.119;
   H[11]=.129;

   H[12]=.13;
   H[13]=.14;
   H[14]=.15;
   H[15]=.16;

   cout << "H:" << endl;
   printmat(H, n+2);
   cout << endl << "Hx:" << endl;
   printmat(Hx, n+1);
   cout << endl;
   
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

   for (int i = 0; i < n+1; i++) {
      // Height
      //    <1.a.1> Hx([1:n+1],[1:n]) = H([2:n+2],[2:n+1])+H([1:n+1],[2:n+1])
      cblas_dcopy(
         n+1,
         (const double*)(H+n+2+i+1),
         n+2,
         Hx+i,
         n+1
      );
      cblas_daxpy(
         n+1,
         1,
         (const double*)(H+1+i),
         n+1,
         Hx+i,
         n+1
      );
      //    <1.a.2> Hx([1:n+1],[1:n]) = Hx(i,j)/2
      cblas_dscal(
         n+1,
         .5,
         Hx+i,
         n+1
      );
      // Since i = 1:n+1 and j = 1:n, 
      //    <1.b> Hx(i,j) = Hx(i,j) - dt/(2*dx)*(U(i+1,j+1)-U(i,j+1));
      //    <1.b> Hx([1:n+1],[1:n]) = Hx([1:n+1],[1:n]) - dt/(2*dx)*(U(i+1,j+1)-U(i,j+1));
      //    <1.b> Hx([1:n+1],[1:n]) += - dt/(2*dx)*(U(i+1,j+1)-U(i,j+1));
      // In other words:
      //    <1.b.1> swap = U(i+1,j+1) ;
      //    <1.b.2> swap -= U(i,j+1) ;
      //    <1.b.3> Hx([1:n+1],[1:n]) += - dt/(2*dx)*swap;
   }
   cout << endl;
}


int main(int argc, char** argv) {
   int n = 64;
   double dt = 0.1;
   double dx = 0.1;

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

   double *swap = (double*)malloc(sizeof(*swap) * n+2);

   firstHalfStep(n, dt, dx, H, Hx, Ux, Vx, Hy, Uy, Vy, swap);


   return 0;
}
