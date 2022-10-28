#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <cstring>

using namespace std;

int main(int argc,char *argv[]) {
  int rank, N, P, T;
  string filename;
  double t0 = 0.0, t1 = 0.0, t2 = 0.0, t3 = 0.0, tf = 0.0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

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
  const int NP = ceil(N / P);
  int A[N * N], At[NP * N];
  int V[N], X[N], Xt[NP];

  if(rank == 0){
cout << P << '\n';
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
  }

  // -------------------------------------------------------------------------

  if(rank == 0)
    t0 = MPI_Wtime();
  
  // NOTE: Send distributed rows to all p available
  MPI_Bcast(&V, N, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(&A, NP * N, MPI_INT, &At, NP * N, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  t1 = MPI_Wtime();

  // -------------------------------------------------------------------------
  
  // NOTE: Local multiply
  memset(Xt, 0, sizeof(Xt));
  #pragma omp parallel for collapse(2) num_threads(T)
  for(i = 0; i < NP; i++) {
    for(j = 0; j < N; j++) {
      #pragma omp atomic
      Xt[i] += At[i * N + j] * V[j];
    }
  }
  
  t2 = MPI_Wtime();

  // -------------------------------------------------------------------------

  // NOTE: Gather array in master
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Gather(&Xt, NP, MPI_INT, &X, NP, MPI_INT, 0, MPI_COMM_WORLD);

  t3 = MPI_Wtime();

  // -------------------------------------------------------------------------

  if(rank == 0) {
    tf = MPI_Wtime();
#ifdef _TEST
    printf("R:\n");
    for(i = 0; i < N; i++)
      printf("%3d ", X[i]);
    printf("\n");

    printf("\n");
    printf("Time for Scatter: %.4f\n", 1000 * (t1 - t0));
    printf("Time for Gather: %.4f\n", 1000 * (t3 - t2));
    printf("Time for Communication: %.4f\n", 1000 * (t1 - t0 + t3 - t2));
    printf("Time for Computation: %.4f\n", 1000 * (t2 - t1));
    printf("Total Time: %.4f\n", 1000 * (tf - t0));
    printf("\n");
#endif

#ifdef _DATA
    ofstream output;
    output.open(filename, ios::app);
    output << fixed << setprecision(4) << 1000 * (t1 - t0 + t3 - t2) << ", " << 1000 * (t2 - t1) << ", " << 1000 * (tf - t0) << "\n";
    output.close();
#endif
  }

  MPI_Finalize();
}
