#include <iostream>
#include <omp.h>

using namespace std;

#define LIMIT 50000000

void imprimir_array(int *A, int N);
double Sortseq(int *A, int N);
double SortParallel(int *A, int N);
double SortEvenOdd(int *A, int N);
void Swap(int *X, int *Y);
void leer_array(int *array, int *array2, int *array3, int *limite, char *filename);
bool comparar_arrays(int *array1, int *array2, int *array3, int *limite);

int *numeros, *numeros2, *numeros3;
int size = LIMIT;
int p = 4;

int main(int argc, char *argv[])
{
  if (argc < 3)
  { 
    // No hay archivo, procesos, ni tamaño
    exit(1);
  }

  p = atoi(argv[2]);

  double t1, t2, t3;

  numeros = new int[size];
  numeros2 = new int[size];
  numeros3 = new int[size];

  // se leen los numeros de un archivo
  leer_array(numeros, numeros2, numeros3, &size, argv[1]);
#ifdef _DEBUG
  // se imprime el array
  imprimir_array(numeros, size);
#endif
  // se ordena el array secuencialmente
  t1 = Sortseq(numeros, size);
  // se divide el array en p partes y se ordena cada parte, luego se realiza un merge
  t2 = SortParallel(numeros2, size);
  // se ordena el array con el algoritmo EvenOdd
  t3 = SortEvenOdd(numeros3, size);
  //  se imprime el array ordenado
#ifdef _DEBUG
  printf("\nArray ordenado Seq:\n");
  imprimir_array(numeros, size);
#endif
  printf("Execution time in ms sequential: %f\n", 1000.0*t1);
#ifdef _DEBUG  
  printf("\nArray ordenado BubbleSort en cada subarray, luego un merge:\n");
  imprimir_array(numeros2, size);
#endif
  printf("Execution time in ms parallel: %f\n", 1000.0*t2);
#ifdef _DEBUG
  printf("\nArray ordenado Odd - Even:\n");
  imprimir_array(numeros3, size);
#endif
  printf("Execution time in ms Odd - Even: %f\n", 1000.0*t3);

  // se comparan los arrays
  if (comparar_arrays(numeros, numeros2, numeros3, &size))
    printf("\nArrays iguales\n");
  else
    printf("\nArrays diferentes\n");

  delete numeros;
  delete numeros2;
  delete numeros3;

  return 0;
}

void leer_array(int *array, int *array2, int *array3, int *size, char *filename)
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
    array[counter++] = numero; // dar valores al array
    if (counter >= *size)      // muchos numeros para el array
      cout << "...muchos numeros para el array\n";
  }
  fclose(opened);

  *size = --counter; // ajusta el tamaño
  cout << *size << " numeros leidos" << endl;

  // se copia paralelamente los array
  int i = 0;
#pragma omp parallel for num_threads(p) shared(array, array2, array3) private(i)
  for (i = 0; i < *size; i++)
  {
    array2[i] = array[i];
    array3[i] = array[i];
  }
}

void imprimir_array(int *A, int N)
{
  int i;
  cout << "[";
  for (i = 0; i < N; i++)
    cout << A[i] << " ";
  cout << "]\n";
}

double Sortseq(int *A, int N)
{
  int i, j;
  double t1, t2;
  t1 = omp_get_wtime();
  for (i = 0; i < N - 1; i++)
    for (j = 0; j < N - i - 1; j++)
      if (A[j] > A[j + 1])
        Swap(&A[j], &A[j + 1]);
  t2 = omp_get_wtime();
  return t2 - t1;
}

void SortHelper(int *A, int lo, int hi)
{
  for (int i = lo; i < hi; i++)
    for (int j = lo; j < hi - i + lo; j++)
      if (A[j] > A[j + 1])
        Swap(&A[j], &A[j + 1]);
}

double SortParallel(int *A, int N)
{
  int i, j;
  double t1, t2;
  // Dividir array en p procesos y ordenar cada subarray
  int ls[p], rs[p];
  t1 = omp_get_wtime();
  for (int i = 0; i < p; i++)
  {
    ls[i] = i * (N / p);
    rs[i] = (i + 1) * (N / p) - 1;
  }
#pragma omp parallel for shared(A, ls, rs) private(i, j) num_threads(p)
  for (int i = 0; i < p; i++)
    SortHelper(A, ls[i], rs[i]);

  // Merge between subarrays
  int *B = new int[N];
  int idx = 0;
  int idxs[p];
  for (int i = 0; i < p; i++)
    idxs[i] = ls[i];
  while (idx < N)
  {
    int min = 1000000;
    int minIdx = -1;
    for (int i = 0; i < p; i++)
    {
      if (idxs[i] <= rs[i] && A[idxs[i]] < min)
      {
        min = A[idxs[i]];
        minIdx = i;
      }
    }
    B[idx++] = min;
    idxs[minIdx]++;
  }


  // Copy merged array to original array
  for (int i = 0; i < N; i++)
    A[i] = B[i];

  delete[] B;
  t2 = omp_get_wtime();
  return t2 - t1;
}

double SortEvenOdd(int *A, int N)
{
  int i, phase;
  double t1, t2;
  t1 = omp_get_wtime();
#pragma omp parallel num_threads(p) shared(A, N) private(i, phase)
  {
    for (phase = 0; phase < N; phase++)
    {
      if (phase % 2 == 0)
      {
#pragma omp for
        for (i = 1; i < N; i += 2)
          if (A[i - 1] > A[i])
            Swap(&A[i - 1], &A[i]);
      }
      else
      {
#pragma omp for
        for (i = 1; i < N - 1; i += 2)
          if (A[i] > A[i + 1])
            Swap(&A[i + 1], &A[i]);
      }
    }
  }
  t2 = omp_get_wtime();
  return t2 - t1;
}

void Swap(int *X, int *Y)
{
  int temp = *X;
  *X = *Y;
  *Y = temp;
}

bool comparar_arrays(int *A, int *B, int *C, int *size)
{
  int i;
  for (i = 0; i < *size; i++)
    if (A[i] != B[i] || A[i] != C[i])
      return false;
  return true;
}