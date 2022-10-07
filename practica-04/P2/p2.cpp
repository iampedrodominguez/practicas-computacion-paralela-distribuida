#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <unistd.h>
#include <iomanip>

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

int main(int argc, char **argv){

  //Initialize variables
  int i,n,*w, threads;
  n = 1000000;
  threads = atoi(argv[1]);
  w = new int[n];

  omp_set_num_threads(threads);

  for(i = 0; i<n; i++) w[i] = rand()%100;
  
  //Get parallel execution time
  auto t0 = std::chrono::steady_clock::now();

  #pragma omp parallel
  {
    #pragma omp single
    qs(w,0,n-1,1);
  }

  auto t1 = std::chrono::steady_clock::now();
  auto dtime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

  // Save parallel time in .csv
  std::ofstream myfile;
  myfile.open("p2.csv", std::ios::app);
  myfile << ", " << std::fixed << std::setprecision(4) << dtime;
  myfile.close();
  
  return 0;
} 
