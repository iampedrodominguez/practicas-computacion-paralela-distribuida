#include <iostream>
#include <omp.h>
#include <math.h>
#include <chrono>
#include <unistd.h>

void swap(int *x, int *y){
  int temp = *x;
  *x = *y;
  *y = temp;
}

int separate(int *x, int low, int high){
  int i, pivot, last;
  pivot = x[low];
  swap(x+low, x+high);
  last = low;
  for(i = low; i<high; i++){
    if(x[i] <= pivot){
      swap(x+last, x+i);
      last += 1;
    }
  }
  swap(x+last, x+high);
  return last;
}

void qs(int *z, int zstart, int zend, int firstcall){
  {
    int pivot;
    
    if (firstcall == 1) 
    {
      qs(z, 0, zend, 0);
    
    } 
    else
    {
      if (zstart < zend) 
      {
      	pivot = separate(z, zstart, zend);
	      #pragma omp task
        {
          //printf("Thread working in task: %i\n", omp_get_thread_num());
          qs(z, zstart, pivot - 1, 0);
        }

        #pragma omp task 
        {
          //printf("Thread working in task: %i\n", omp_get_thread_num());
	        qs(z, pivot + 1, zend, 0);
        }
      }
    }
  }
}

template <typename T>
void prefix_sum(T *A, int n)
{
    int i, j;
    T **B, **C;
    B = new T *[n + 1];
    C = new T *[n + 1];
    for (i = 0; i <= n; i++)
    {
        B[i] = new T[n + 1];
        C[i] = new T[n + 1];
    }

    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            B[i][j] = C[i][j] = 0;

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
            A[i - 1] = C[0][i];
    }
    for (i = 0; i <= n; i++)
    {
        delete[] B[i];
        delete[] C[i];
    }
    delete[] B;
    delete[] C;
}

template <typename T>
void scan_left(T *A, T x, int n)
{
    int i, j;
    T **B, **C;
    B = new T *[n + 1];
    C = new T *[n + 1];
    for (i = 0; i <= n; i++)
    {
        B[i] = new T[n + 1];
        C[i] = new T[n + 1];
    }

    for (i = 0; i <= n; i++)
        for (j = 0; j <= n; j++)
            B[i][j] = C[i][j] = 0;
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
    for (i = 0; i <= n; i++)
    {
        delete[] B[i];
        delete[] C[i];
    }
    delete[] B;
    delete[] C;
}