// omp scan left
#include <iostream>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <unistd.h>
#include <vector>

typedef double dtype;

void scan_left(dtype *A, dtype x, int n)
{
    int i, j;
    dtype **B, **C;
    B = new dtype *[n + 1];
    C = new dtype *[n + 1];
    for (i = 0; i <= n; i++)
    {
        B[i] = new dtype[n + 1];
        C[i] = new dtype[n + 1];
    }

    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            B[i][j] = C[i][j] = 0;
    auto t0 = std::chrono::steady_clock::now();

#pragma omp parallel num_threads(8)
    {
#pragma omp for
        for (i = 1; i <= n; i++)
            B[0][i] = A[i - 1];

        for (int i = 1; i <= (int)log2(n); i++)
        {
#pragma omp for
            for (j = 1; j <= n / ((int)pow(2, i)); j++)
                B[i][j] = B[i - 1][2 * j - 1] + B[i - 1][2 * j];
        }
        for (int i = (int)log2(n); i >= 0; i--)
        {
#pragma omp for
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
#pragma omp for
        for (i = 1; i <= n; i++)
            A[i - 1] = C[0][i] + x;
    }
    auto t1 = std::chrono::steady_clock::now();

    std::cout << "Tiempo paralelo: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() << " ms" << std::endl;

    for (i = 0; i <= n; i++)
    {
        delete[] B[i];
        delete[] C[i];
    }
    delete[] B;
    delete[] C;
}

int main(int argc, char *argv[])
{
    const int N = argv[1] ? atoi(argv[1]) : 8;
    const int x = argv[2] ? atoi(argv[2]) : 0;

    // int max_threads = omp_get_max_threads();
    std::cout << "N = " << N << std::endl;
    dtype *A = new dtype[N];
    for (int i = 0; i < N; i++)
        A[i] = i+1;
#ifdef _DEBUG
    for (int i = 0; i < N; i++)
        std::cout << A[i] << " ";
    std::cout << "\n";
#endif
    scan_left(A, x, N);

#ifdef _DEBUG
    for (int i = 0; i < N; i++)
        std::cout << A[i] << " ";
    std::cout << "\n";
#endif

    delete[] A;
    return 0;
}