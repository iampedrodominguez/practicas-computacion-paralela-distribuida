#include <stdio.h>
#include <omp.h>
#include <math.h>

const int N = 2;
int p = 8;

void normaliza(double A[N][N])
{
    int i, j;
    double suma = 0.0, factor;

    #pragma omp parallel num_threads(p) 
    {
        #pragma omp for private(i, j) reduction(+:suma)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                suma += A[i][j] * A[i][j];
        
    }
    factor = 1.0 / sqrt(suma);

    #pragma omp parallel num_threads(p) 
    {
        #pragma omp for private(i, j)
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                A[i][j] *= factor;
    }
}

int main(void)
{
    double A[N][N];
    int i, j;    

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            A[i][j] = i + j;

    normaliza(A);

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            printf("%f ", A[i][j]);
        printf("\n");
}
