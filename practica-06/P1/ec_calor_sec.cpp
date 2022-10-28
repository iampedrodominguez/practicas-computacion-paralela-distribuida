#include <cmath>
#include <ctime>
#include <iostream>
#include <mpi.h>

double frontera(double x, double tiempo) {
  double limite;
  if(x < 0.5)
    limite = 100.0 + 10.0 * sin(tiempo);
  else
    limite = 75.0;
  
  return limite;
}

double inicial(double x, double tiempo) {
  double limite;
  limite = 95.0;
  
  return limite;
}

int main(int argc, char *argv[]) {
  int rank, P;

  int i, j, j_min = 0, j_max = 400, tag, n = 10;
  double k = 0.002;
  double tiempo, dt, tmax = 10.0, tmin = 0.0, tnew;
  double u[n], unew[n], x[n], dx;
  double x_max = 1.0, x_min = 0.0;
  MPI_Request reqs[4];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &P);

  dt = (tmax - tmin) / (double)(j_max - j_min);
  dx = (x_max - x_min) / (double)(n - 1);
	x[0] = 0;
	for(i = 1; i < n; i++) {
    x[i] = x[i - 1] + dx;
  }

  if(rank == 0) {
    // Inicializacion
    tiempo = tmin;
    u[0] = 0.0;
    for(i = 1; i <= n; i++ )
      u[i] = inicial(x[i], tiempo);
    u[n + 1] = 0.0;
  }

  // Valores de temperatura al siguiente intervalo de tiempo
  for(j = 1; j <= j_max; j++) {
    tnew += dt;
    // actualizar temperatura
    MPI_Isend(&u[rank], 1, MPI_DOUBLE, rank + 1, 8, MPI_COMM_WORLD, &reqs[0]);
    MPI_Isend(&u[rank], 1, MPI_DOUBLE, rank - 1, 8, MPI_COMM_WORLD, &reqs[1]);

    MPI_Irecv(&u[rank + 1], 1, MPI_DOUBLE, rank + 1, 8, MPI_COMM_WORLD, &reqs[2]);
    MPI_Irecv(&u[rank - 1], 1, MPI_DOUBLE, rank - 1, 8, MPI_COMM_WORLD, &reqs[3]);

    unew[rank] = u[rank] + (dt * k / dx / dx) * (u[rank - 1] - 2.0 * u[rank] + u[rank + 1]); 

    // aplicar condiciones de frontera
    if(rank == 0) {
      unew[1] = frontera(x[1], tnew);
      unew[n] = frontera(x[n], tnew);
    }

    // Actualizar tiempo y temperatura
    tiempo = tnew;

    for ( i = 1; i <= n; i++ ){
      u[i] = unew[i];
      if(j == j_max)
        printf("%f %f %f\n", tiempo, x[i], u[i]);
    }
  }

  MPI_Finalize();
}
