#include <stdio.h>
#include <omp.h>
#include <math.h>

const int N = 3;
int p = 8;

/*
    Creamos dos regiones paralelas, una para calcular la suma total de los cuadrados y otra para normalizar la matriz.
    En este caso, no cambia el performance con el nowait por la barrera implicita que se crea al final de la region paralela.
*/
void normaliza(double A[N][N])
{
    int i, j;
    double suma = 0.0, factor;

    #pragma omp parallel num_threads(p) 
    {
        int id = omp_get_thread_num();
        #pragma omp for private(i, j) reduction(+:suma)
        for (i = 0; i < N; i++){
            for (j = 0; j < N; j++)
                suma += A[i][j] * A[i][j];
            printf("Thread %d: suma = %f\n", id, suma);
        }
    }
    factor = 1.0 / sqrt(suma);
    
    #pragma omp parallel num_threads(p) 
    {    
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
