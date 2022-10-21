#include <mpi.h>
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc,char *argv[]) {
  int rank, N, P;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

  if(argc == 1) {
    N = 10;
  }
  else {
    N = atoi(argv[1]);
  }

  int i, j, NP;
  NP = ceil(N / P);
  int A[N * N], At[N * N];
  int V[N], X[N], Xt[N];

  if(rank == 0){
    for(i = 0; i < N; i++){
      for(j = 0; j < N; j++)
        A[i * N + j] = i;
      V[i] = 1;
    }
#ifdef TEST
    printf("Matrix A:\n");
    for(i = 0; i < N; i++) {
      for(j = 0; j < N; j++)
        printf("%d ", A[i * N + j]);
      printf("\n");
    }
    printf("Vector V:\n");
    for(i = 0; i < N; i++)
      printf("%d ", V[i]);
    printf("\n");
#endif
  }

  // -------------------------------------------------------------------------

  MPI_Bcast(&V, N, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(&A, NP * N, MPI_INT, &At, NP * N, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);

  for(i = 0; i < NP; i++) {
    for(j = 0; j < N; j++)
      printf("%d ", At[i * N + j]);
    printf("\n");
  }

  memset(Xt, 0, sizeof(Xt));
  #pragma omp parallel for collapse(2)
  for(i = 0; i < NP; i++) {
    for(j = 0; j < N; j++) {
      #pragma omp atomic
      Xt[i] += At[i * N + j] * V[j];
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  // for(i = 0; i < NP; i++)
    MPI_Gather(&Xt, NP, MPI_INT, &X, NP, MPI_INT, 0, MPI_COMM_WORLD);

  // -------------------------------------------------------------------------

  if(rank == 0) {
    printf("Resultado:\n");
    for(i = 0; i < N; i++)
      printf("%d ", X[i]);
    printf("\n");
  }

  MPI_Finalize();
}
