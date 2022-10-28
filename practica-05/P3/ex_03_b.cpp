#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <cstring>

using namespace std;

int main(int argc,char *argv[]) {
  int N, T;
  string filename;

  if(argc != 4) {
    filename = "./data/data.csv";
    N = 10;
    T = 2;
  }
  else {
    filename = argv[1];
    N = atoi(argv[2]);
    T = atoi(argv[3]);
  }

  int i, j;
  int A[N * N];
  int V[N], X[N];

  // NOTE: Read Matrix and Vector for master
  #pragma omp parallel for collapse(2) num_threads(T)
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++)
      A[i * N + j] = i;
  }
  #pragma omp parallel for num_threads(T)
  for(i = 0; i < N; i++)
    V[i] = 1;

#ifdef _TEST
  printf("A:\n");
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++)
      printf("%3d ", A[i * N + j]);
    printf("\n");
  }
  printf("V:\n");
  for(i = 0; i < N; i++)
    printf("%3d ", V[i]);
  printf("\n");
#endif

  // -------------------------------------------------------------------------

  auto t0 = chrono::steady_clock::now();
  
  // -------------------------------------------------------------------------
  
  // NOTE: Local multiply
  memset(X, 0, sizeof(X));
  #pragma omp parallel for collapse(2) num_threads(T)
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      #pragma omp atomic
      X[i] += A[i * N + j] * V[j];
    }
  }
  
  // -------------------------------------------------------------------------

  auto t1 = chrono::steady_clock::now();

  // -------------------------------------------------------------------------

  auto tf = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
#ifdef _TEST
  printf("R:\n");
  for(i = 0; i < N; i++)
    printf("%3d ", X[i]);
  printf("\n");

  printf("\n");
  printf("Time for Computation: %.4f\n", tf);
  printf("\n");
#endif

#ifdef _DATA
  ofstream output;
  output.open(filename, ios::app);
  output << fixed << setprecision(4) << tf << "\n";
  output.close();
#endif
}
