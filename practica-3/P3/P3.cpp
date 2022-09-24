#include <iostream>
#include <math.h>
#include <mpi.h>
#include <fstream>
#include <iomanip>

int main(int argc, char **argv)
{
  // NOTE: Init
  int P, rank, sm;
  std::string filename;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &P);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  double t0 = 0.0, t1 = 0.0, t2 = 0.0, t3 = 0.0, tf = 0.0;

#ifdef _DATA
  if(rank == 0)
    filename = argv[1];
#endif
  
#ifdef _TEST
  const int N = 4;
#else
  const int N = 1150;
#endif
  const int N_matrix_local = N * N / P;
  int A[N * N], At[N_matrix_local], V[N], Vt[N], X[N], Xt[N];

  if (rank == 0)
  {
    // NOTE: Read Matrix for master
    int val = 0;
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
        A[i * N + j] = val++;
      V[i] = i * i;
    }
#ifdef _TEST
    printf("Print Matrix and Vector -------------------------\n");
    printf("Matrix\n");
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
        printf("%4d ", A[i * N + j]);
      printf("\n");
    }
    printf("Vector\n");
    for (int i = 0; i < N; i++)
      printf("%4d ", V[i]);
    printf("\n");
    printf("--------------------------------------------------\n");
#endif
  }

  if (rank == 0)
    t0 = MPI_Wtime();
  
  // NOTE: Send distributed rows to all p available
  MPI_Scatter(A, N_matrix_local, MPI_INT, At, N_matrix_local, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(V, N, MPI_INT, 0, MPI_COMM_WORLD);

  t1 = MPI_Wtime();

  // NOTE: Local multiply
#ifdef _TEST
  printf("Process %d\n", rank);
  printf("Matrix\n");
  for (int i = 0; i < N / P; i++)
  {
    for (int j = 0; j < N; j++)
      printf("%4d ", At[i * N + j]);
    printf("\n");
  }
  printf("Vector\n");
  for (int i = 0; i < N; i++)
    printf("%4d ", V[i]);
  printf("\n\n");
#endif

  for (int i = 0; i < N / P; i++)
  {
    Xt[i] = 0;
    for (int j = 0; j < N; j++)
      Xt[i] += (At[i * N + j] * V[j]);
#ifdef _TEST
    printf("Xt[%d] from process %d: %d\n", i, rank, Xt[i]);
#endif
  }

  t2 = MPI_Wtime();
  // NOTE: Gather array in master
  MPI_Gather(Xt, N / P, MPI_INT, X, N / P, MPI_INT, 0, MPI_COMM_WORLD);
  t3 = MPI_Wtime();
  if (rank == 0)
  {
#ifdef _TEST
    printf("Master: \n");
    for (int i = 0; i < N; i++)
      printf("%4d ", X[i]);
    printf("\n");
#endif
    tf = MPI_Wtime();
#ifdef _TEST
    printf("Time for Scatter: %.4f\n", 1000 * (t1 - t0));
    printf("Time for Gather: %.4f\n", 1000 * (t3 - t2));
    printf("Time for Communication: %.4f\n", 1000 * (t1 - t0 + t3 - t2));
    printf("Time for Computation: %.4f\n", 1000 * (t2 - t1));
    printf("Total Time: %.4f\n", 1000 * (tf - t0));
#endif

#ifdef _DATA
    // Open data.txt and add times comm, comp, total
    std::ofstream myfile;
    myfile.open(filename, std::ios_base::app);
    myfile << std::fixed << std::setprecision(4) << 1000 * (t1 - t0 + t3 - t2) << "," << 1000 * (t2 - t1) << "," << 1000 * (tf - t0) << "\n";
    myfile.close();
#endif
  }
  MPI_Finalize();
}

//make e3_data run_e3_data