#include <iostream>
#include <omp.h>

using namespace std;

#define LIMIT 1000

void imprimir_array(int A[], int N);
void Sort(int A[], int N);
void Sortseq(int A[], int N);
void SortEvenOdd(int A[], int N);
void Swap(int *X, int *Y);
void leer_array(int array[], int array2[], int *limite, char *filename);

int numeros[LIMIT]; // tamanho maximo del array
int numeros2[LIMIT]; // tamanho maximo del array
int size = LIMIT;

int main(int argc, char *argv[])
{
  if (argc < 2)
  { // no hay archivo => exit
    exit(1);
  }

  // se leen los numeros de un archivo
  leer_array(numeros, numeros2, &size, argv[1]);
  // se imprime el array
  imprimir_array(numeros, size);
  // se ordena el array secuencialmente
  Sortseq(numeros2, size);
  SortEvenOdd(numeros, size);
  //Sort(numeros, size);
  // se imprime el array ordenado
  printf("\nArray ordenado Seq:\n");
  imprimir_array(numeros2, size);
  printf("Array ordenado Odd - Even:\n");
  imprimir_array(numeros, size);

  bool equals = true;
  for (int i = 0; i < size; i++)
  {
    if (numeros[i] != numeros2[i])
    {
      equals = false;
      break;
    }
  }
  if(equals)
    printf("\nArrays ordenados iguales\n");
  else
    printf("\nERROR: Arrays diferentes\n");
  return 0;
}

void leer_array(int array[], int array2[], int *size, char *filename)
{
  FILE *opened; // file
  char buf[30];
  int numero;      // numero de elementos leidos
  int counter = 0; // counter for elems in file

  opened = fopen(filename, "r");

  while (!feof(opened) && counter < *size)
  {                            // lee hasta EOF o muchos elementos
    fgets(buf, 30, opened);    // lee una linea, max 30 caracteres
    numero = atoi(buf);        // convierte a entero
    array2[counter] = numero; // dar valores al array
    array[counter++] = numero; // dar valores al array
    if (counter >= *size)      // muchos numeros para el array
      cout << "...muchos numeros para el array\n";
  }
  fclose(opened);

  *size = --counter; // ajusta el tamaño
  cout << *size << " numeros leidos" << endl;
}

void imprimir_array(int A[], int N)
{
  int i;
  cout << "[";
  for (i = 0; i < N; i++)
    cout << A[i] << " ";
  cout << "]\n";
}

void Sortseq(int A[], int N)
{
  int i, j;

  for (i = 0; i < N - 1; i++)
    for (j = 0; j < N - i - 1; j++)
      if (A[j] > A[j + 1])
        Swap(&A[j], &A[j + 1]);
}

void SortHelper(int A[], int lo, int hi)
{
  printf("lo: %d, hi: %d\n", lo, hi);
  int i, j;

  for (i = lo; i < hi; i++)
    for (j = lo; j < hi - 1; j++)
      if (A[j] > A[j + 1])
        Swap(&A[j], &A[j + 1]);
}

void Sort(int A[], int N)
{
  int i, j;
  // Dividir array en p procesos y ordenar cada subarray
  int p = 4;
  printf("p: %d\n", p);
  int ls[p], rs[p];
  for(int i=0; i<p; i++){
    ls[i] = i*(N/p);
    rs[i] = (i+1)*(N/p) - 1;
  }
  
  #pragma omp parallel for shared(A, ls, rs) private(i, j) num_threads(p)
  for(int i=0; i<p; i++){
    SortHelper(A, ls[i], rs[i]);
  }
  
  // Print each sub array
  for(int i=0; i<p; i++){
    printf("Subarray %d: ", i);
    for(int j=ls[i]; j<=rs[i]; j++)
      printf("%d ", A[j]);
    printf("\n");
  }

  //TODO: Merge subarrays
  int indexes[p];
  for(int i=0; i<p; i++)
    indexes[i] = ls[i];
  
}

void SortEvenOdd(int A[], int N)
{
  int i, phase;
  #pragma omp parallel num_threads(4) shared(A, N) private(i, phase)
  {
    for(phase = 0; phase < N; phase++){
      if(phase % 2 == 0){
        #pragma omp for
        for(i = 1; i < N; i += 2)
          if(A[i - 1] > A[i])
            Swap(&A[i - 1], &A[i]);
      }else{
        #pragma omp for
        for(i = 1; i < N - 1; i += 2)
          if(A[i] > A[i + 1])
            Swap(&A[i + 1], &A[i]);
      }
    }
  }
}

void Swap(int *X, int *Y)
{
  int temp = *X;
  *X = *Y;
  *Y = temp;
}
