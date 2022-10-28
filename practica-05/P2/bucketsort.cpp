//#include <regex>

#include<bits/stdc++.h>
#include <omp.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

#include <iomanip>
using namespace std;
void bucketSort(float *arr, int &n, int bucket_size)
{
    // Crear buckets
    std::vector<float> bucket[bucket_size];
    int i, j, bucket_index, index = 0;

    // asignar elementos a los buckets
    for (i = 0; i < n; ++i)
    {
        bucket_index = bucket_size * arr[i] / 1000;
        bucket[bucket_index].push_back(arr[i]);
    }

    // ordenar buckets
    for (j = 0; j < bucket_size; ++j)
        sort(bucket[j].begin(), bucket[j].end());

    // Concatenar buckets en arr[]
    for (i = 0; i < bucket_size; i++)
    {
        for (j = 0; j < bucket[i].size(); j++)
            arr[index++] = bucket[i][j];
    }
}

void bucketSortParallel(float *arr, int &n, int bucket_size, int number_of_threads)
{
    // Crear buckets
    std::vector<float> bucket[bucket_size];
    int i, j, bucket_index, index = 0;
    omp_set_num_threads(number_of_threads);

    
// asignar elementos a los buckets
#pragma omp parallel 
     {   
        #pragma omp for private(i,bucket_index) 
        for (i = 0; i < n; ++i)
        {
            bucket_index = bucket_size * arr[i] / 1000;
            #pragma omp critical
            bucket[bucket_index].push_back(arr[i]);
        }
        
        #pragma omp barrier
// ordenar buckets
         #pragma omp for private(j)
        for (j = 0; j < bucket_size; ++j)
            sort(bucket[j].begin(), bucket[j].end());
// Concatenar buckets en arr[]
     }

        for (i = 0; i < bucket_size; i++) {
    for (j = 0; j < bucket[i].size(); j++)
      arr[index++] = bucket[i][j];
  }
    
}

int main(int argc, char *argv[])
{
    int i, n = pow(2, 19);
    float *randArray;

    srand((int)time(0));

    randArray = new float[n];
    
    int num_of_iterations = 10;

    for (int i = 0; i < n; ++i){
        randArray[i] = (float)rand() / (float)(RAND_MAX / 999.0);
    }

    int num_of_process = 12;
    int* total_of_process;
    total_of_process = new int[num_of_process];
    
    int m=1;
    for(int i=0;i<num_of_process;i++){
       total_of_process[i]=m;
       m*=2;
    }
    
    double * answer = new double[num_of_process]; 
    for(int i=0;i<num_of_process;i++){
        double sum=0;
        for(int j=0;j<10;j++){
           double t1, t2;
           int s = total_of_process[i];
           t1 = omp_get_wtime();
           
           bucketSortParallel(randArray, n, 8,s);
           t2 = omp_get_wtime();
           sum = sum + (t2-t1);
        }
        sum = sum/10.0;
        answer[i] = sum;

    }
    for(i =0;i<num_of_process;i++){
        printf("%f8 ", answer[i]);
    }
    delete[] randArray;
    delete[] total_of_process;
    delete[] answer;
}
