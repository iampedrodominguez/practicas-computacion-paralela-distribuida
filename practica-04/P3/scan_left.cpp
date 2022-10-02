// omp scan left
#include <iostream>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <unistd.h>
#include <vector>

typedef unsigned long long int dtype;

void prefix_sum(dtype *A, int n)
{
    int i, j;
    dtype **B, **C;
    B = new dtype *[n + 1]; 
    C = new dtype *[n + 1];
    for(i = 0; i <= n; i++){
        B[i] = new dtype[n + 1];
        C[i] = new dtype[n + 1];
    }
    for(i = 0; i <= n; i++)
        for(j = 0; j <= n; j++)
            B[i][j] = C[i][j] = 0;

    /*std::vector<std::vector<dtype>> B(n + 1, std::vector<dtype>(n + 1, 0));
    std::vector<std::vector<dtype>> C(n + 1, std::vector<dtype>(n + 1, 0));*/
    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            B[i][j] = C[i][j] = 0;
    auto t0 = std::chrono::steady_clock::now();

#pragma omp parallel num_threads(8)
    {
#pragma omp for
        for (int i = 1; i <= n; i++)
            B[0][i] = A[i - 1];

        for (int i = 1; i <= (int)log2(n); i++)
        {
#pragma omp for 
            for (int j = 1; j <= n / ((int)pow(2, i)); j++)
                B[i][j] = B[i - 1][2 * j - 1] + B[i - 1][2 * j];
        }
        for (int i = (int)log2(n); i >= 0; i--)
        {
#pragma omp for 
            for (int j = 1; j <= n / ((int)pow(2, i)); j++)
            {
                if (j % 2 == 0)
                    C[i][j] = C[i + 1][j / 2];
                else if (j == 1)
                    C[i][j] = B[i][j];
                else
                    C[i][j] = C[i + 1][(j - 1) / 2] + B[i][j];
            }
        }
    }
    auto t1 = std::chrono::steady_clock::now();

#ifdef _DEBUG
    for (i = 0; i < n; i++)
    {
        std::cout << C[0][i + 1] << " ";
    }
    std::cout << "\n";
#else
    std::cout << "last value parallel: " << C[0][n] << "\n";
#endif

    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            B[i][j] = C[i][j] = 0;

    auto t2 = std::chrono::steady_clock::now();
    for (i = 1; i <= n; i++)
        B[0][i] = A[i - 1];
    
    for (i = 1; i <= (int)log2(n); i++)
        for (int j = 1; j <= n / ((int)pow(2, i)); j++)
            B[i][j] = B[i - 1][2 * j - 1] + B[i - 1][2 * j];
    
    for (i = (int)log2(n); i >= 0; i--)
    {
        for (j = 1; j <= n / ((int)pow(2, i)); j++)
        {
            if (j % 2 == 0)
                C[i][j] = C[i + 1][j / 2];
            else if (j == 1)
                C[i][j] = B[i][j];
            else
                C[i][j] = C[i + 1][(j - 1) / 2] + B[i][j];
        }
    }
    auto t3 = std::chrono::steady_clock::now();

#ifdef _DEBUG
    for (int i = 0; i < n; i++)
    {
        std::cout << C[0][i + 1] << " ";
    }
    std::cout << "\n\n";
#else
    std::cout << "last value secuencial: " << C[0][n] << "\n";
#endif

    std::cout << "Tiempo paralelo: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << " ms" << std::endl;
    std::cout << "Tiempo secuencial: " << std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count() << " ms" << std::endl;

    for(i = 0; i <= n; i++){
        delete[] B[i];
        delete[] C[i];
    }
    delete[] B;
    delete[] C;
}

int main(int argc, char *argv[])
{
    const int N = argv[1] ? atoi(argv[1]) : 8;
    
    //int max_threads = omp_get_max_threads();
    std::cout << "N = " << N << std::endl;
    dtype *A = new dtype[N];
    for (int i = 0; i < N; i++)
        A[i] = i;
#ifdef _DEBUG
    for (int i = 0; i < N; i++)
        std::cout << A[i] << " ";
    std::cout << "\n";
#endif
    prefix_sum(A, N);
    
    delete[] A;
    return 0;
}