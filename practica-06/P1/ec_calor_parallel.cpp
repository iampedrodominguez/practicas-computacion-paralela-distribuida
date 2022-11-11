#include <iostream>
#include <cmath>
#include <ctime>
# include "mpi.h"
double frontera(double x, double tiempo,int id)
{
    double limite;
    if (x < 0.5 || id==0)
        limite = 100.0 + 10.0 * sin(tiempo);
    else
        limite = 75.0;
    return limite;
}

double inicial(double x, double tiempo)
{
    double limite;
    limite = 95.0;
    return limite;
}

int main(int argc, char *argv[])
{   
    int i, j, j_min = 0, j_max = 400, tag, n = 10;
    int rank,n_of_process;
    double k = 0.002;
    double tiempo, dt, tmax = 10.0, tmin = 0.0, tnew;
    double u[n+2], unew[n+2], x[n+2], dx;
    double x_max = 1.0, x_min = 0.0;
    dt = (tmax - tmin) / (double)(j_max - j_min);
    dx = (x_max - x_min) / (double)(n - 1);
    x[0] = 0;
    for (i = 1; i <= n; i++)
    {
        x[i] = x[i - 1] + dx;
    }

    //  Inicializacion.
    tiempo = tmin;
    u[0] = 0.0;
    for (i = 1; i <= n; i++)
        u[i] = inicial(x[i], tiempo);
    u[n + 1] = 0.0;
    MPI_Init ( &argc, &argv );
    MPI_Comm_rank ( MPI_COMM_WORLD, &rank);
    MPI_Comm_size ( MPI_COMM_WORLD, &n_of_process);
    MPI_Status s;
    // Valores de temperatura al siguiente intervalo de tiempo
    for (j = 1; j <= j_max; j++)
    {
        tnew += dt;
        // actualizar temperatura
        for (i = 1; i <= n; i++)
        {
            unew[i] = u[i] + (dt * k / dx / dx) * (u[i - 1] - 2.0 * u[i] + u[i + 1] );
        }

        // send, recv
        if (rank<n_of_process-1) MPI_Send ( &unew[n], 1, MPI_DOUBLE, rank+1, 1,MPI_COMM_WORLD);
        
        if(rank>0)   MPI_Recv ( &unew[0], 1, MPI_DOUBLE, rank-1, 1,MPI_COMM_WORLD,&s);

        // aplicar condiciones de frontera
        unew[1] = frontera(x[1], tnew,rank);

        // send recv 
        if (0<rank) MPI_Send ( &unew[1], 1, MPI_DOUBLE, rank-1,2,MPI_COMM_WORLD);  
        if (rank<n_of_process-1) MPI_Recv ( &unew[n+1], 1, MPI_DOUBLE, rank+1,2,MPI_COMM_WORLD,&s);
        // aplicar condiciones de frontera
        unew[n] = frontera(x[n], tnew,rank);

        // Actualizar tiempo y temperatura
        tiempo = tnew;
        
        for (i = 1; i <= n; i++)
        {
            u[i] = unew[i];
        }
        
    }
    MPI::Finalize();
    for (i = 1; i <= n; i++)
        {
                printf("%f %f %f\n", tiempo, x[i], u[i]);
        }
    
    return 0;
}
