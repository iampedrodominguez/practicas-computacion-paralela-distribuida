#include <iostream>
#include "P3/quicksort.cpp"

#define x 10

double OpArr(double A[], int B[], double C[], int n){
    int i, j;
    double s1, s2, a, res;
    
    suma_de_prefijos(A, n); // obtiene el array de suma de prefijos de A
    qs(B, 0, n - 1, 1); // ordena el array B
    scan_left(C, x, n); // acumula los valores de elementos de C mas una constante x

    #pragma omp single
    for (i=0; i<n; i++) 
    { /* primer bucle for*/
        s1=0;

        #pragma omp task depend(in: A, B) depend(out: s1)
        for (j=0; j<n; j++) s1+=A[j]*B[j];

        #pragma omp task depend(in: s1) depend(out: A) 
        for (j=0; j<n; j++) A[j]*=s1;
    }
    
    #pragma omp single
    for (i=0; i<n; i++) 
    { /* segundo bucle for */
        s2=0;

        #pragma omp task depend(in: B, C) depend(out: s2)
        for (j=0; j<n; j++) s2+=B[j]*C[j];

        #pragma omp task depend (in: s2) depend(out: C)
        for (j=0; j<n; j++) C[j]*=s2;
    }
    /* calculo final */
    a=s1/s2;
    res =0;
    
    #pragma omp task depend(in: a, C)
    for (i=0; i<n; i++) res+=a*C[i];
    
    return res;
}


int main(int argc, char **argv){
  int i,n,*w;
  n = atoi(argv[1]);
  w = malloc(n*sizeof(int));

  omp_set_num_threads(12);

  for(i = 0; i<n; i++) w[i] = rand()%100;
  for(i=0;i<n;i++) printf("%d\n",w[i]);
  printf("\n");
  
  #pragma omp parallel 
  {
    #pragma omp single
    qs(w,0,n-1,1);
  }

  if(n<250)
    for(i=0;i<n;i++) printf("%d\n",w[i]);
  return 0;
} 