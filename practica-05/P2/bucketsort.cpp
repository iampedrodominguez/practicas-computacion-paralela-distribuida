#include <omp.h>
#include <math.h>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <iomanip>
using namespace std;
void bucketSort(float* arr, int& n, int bucket_size) {
  // Crear buckets      
  std::vector<float> bucket[bucket_size];
  int i, j, bucket_index, index = 0;

  // asignar elementos a los buckets
  for (i = 0; i < n; ++i) {
    	bucket_index = bucket_size*arr[i]/1000;
      	bucket[bucket_index].push_back(arr[i]);
  }

  // ordenar buckets
  for (j = 0; j < bucket_size; ++j)
    sort(bucket[j].begin(), bucket[j].end());

  // Concatenar buckets en arr[]
  for (i = 0; i < bucket_size; i++) {
    for (j = 0; j < bucket[i].size(); j++)
      arr[index++] = bucket[i][j];
  }
}


void bucketSortParallel(float* arr, int& n, int bucket_size) {
  // Crear buckets      
  std::vector<float> bucket[bucket_size];
  int i, j, bucket_index, index = 0;
  omp_set_num_threads(16);
  #pragma omp parallel 
  {
  // asignar elementos a los buckets 
  #pragma omp for private(i,bucket_index)
  for (i = 0; i < n; ++i) {
    	bucket_index = bucket_size*arr[i]/1000;
        #pragma omp critical
      	bucket[bucket_index].push_back(arr[i]);
  }
  #pragma omp barrier

  // ordenar buckets
  #pragma omp for private(j)
  for (j = 0; j < bucket_size; ++j)
    sort(bucket[j].begin(), bucket[j].end());
  // Concatenar buckets en arr[]
  #pragma omp barrier
  int k;
  #pragma omp task private(i,j,k,index) depend(in:i) depend(out:k)
  for (i = 0; i < bucket_size; i++) {
    k=bucket[i].size();
    for (j = 0; j < k; j++)
      
      arr[index++] = bucket[i][j];
      printf("%d ",index);
  }
  }
}

int main(int argc, char *argv[]) {
  int i, n = pow(2, 4);     
  float* randArray;
  //omp_set_num_threads(8);

  srand((int)time(0));
    
  randArray = new float[n];
  
  for(int i = 0; i < n; ++i)
    randArray[i]=(float)rand()/(float)(RAND_MAX/999.0);

  for (i = 0; i < n; ++i) 
    	printf("%1.2f, ", randArray[i]);
	printf("\n");
  auto randArray1=randArray;
  // bucketSort(randArray, n, 8);
  auto t0 = chrono::steady_clock::now();
  bucketSortParallel(randArray, n, 8);
  auto t1 = std::chrono::steady_clock::now();
  auto dtime = std::chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
  cout << "Tiempo paralelo : " << dtime << " ms" << std::endl;
  for (i = 0; i < n; ++i) 
    printf("%1.2f, ", randArray[i]);
  

  delete[] randArray;
}
}
