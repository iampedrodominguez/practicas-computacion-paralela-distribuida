#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

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
          printf("Thread working in task: %i\n", omp_get_thread_num());
          qs(z, zstart, pivot - 1, 0);
        }

        #pragma omp task 
        {
          printf("Thread working in task: %i\n", omp_get_thread_num());
	        qs(z, pivot + 1, zend, 0);
        }
      }
    }
  }
}