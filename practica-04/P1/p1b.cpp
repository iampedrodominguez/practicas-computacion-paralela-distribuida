#include <stdio.h>
#include <omp.h>
#include <math.h>

const int N = 3;
int p = 8;

/*
    En una misma región paralela, calculamos la suma total de los cuadrados y normalizamos la matriz.
    En este caso, no conviene usar el nowait ya que la suma debe calcularse antes de normalizar la matriz.
    Si probamos con el nowait, el resultado es sale incorrecto en algunas ejecuciones.
*/

void normaliza(double A[N][N])
{
    int i, j;
    double suma = 0.0, factor;

    #pragma omp parallel num_threads(p) 
    {
        int id = omp_get_thread_num();
        //#pragma omp for private(i, j) reduction(+:suma) nowait
        #pragma omp for private(i, j) reduction(+:suma)
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                suma += A[i][j] * A[i][j];
            printf("Thread %d: suma = %f\n", id, suma);
        }
        
        factor = 1.0 / sqrt(suma);
        
        #pragma omp for private(i, j)
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                A[i][j] *= factor;
        }
    }
}

int main(void)
{
    double A[N][N];
    int i, j;    

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++){
            A[i][j] = i + j;
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    normaliza(A);

    printf("\n");

    for (i = 0; i < N; i++){
        for (j = 0; j < N; j++)
            printf("%f ", A[i][j]);
        printf("\n");
    }
}
