#include <iostream>
#include "functions.cpp"
#include <fstream>
#include <iomanip>
#include <algorithm>

#define x 0
int P = 4;
std::string filename;

template <typename T, typename U, typename V>
void prnt(T *A, U *B, V *C, int n)
{
  // print A
  std::cout << "A: ";
  for (int i = 0; i < n; i++)
    std::cout << A[i] << " ";
  std::cout << std::endl;

  // print B
  std::cout << "B: ";
  for (int i = 0; i < n; i++)
    std::cout << B[i] << " ";
  std::cout << std::endl;

  // print C
  std::cout << "C: ";
  for (int i = 0; i < n; i++)
    std::cout << C[i] << " ";
  std::cout << std::endl;
}

double OpArr(double *A, int *B, double *C, int n)
{
  int i, j;
  double s1, s2, a, res;

  prefix_sum<double>(A, n); // obtiene el array de suma de prefijos de A

  #pragma omp parallel
  #pragma omp single
  qs(B, 0, n - 1, 1); // ordena el array B

  scan_left<double>(C, x, n); // acumula los valores de elementos de C mas una constante x = 100

#ifdef _DEBUG
  prnt(A, B, C, n);
#endif

  #pragma omp parallel default(none) shared(A, B, n, s1)
  #pragma omp single private(i)
  for(i = 0; i < n; i++) {
    // primer bucle for
    s1 = 0.0;

    #pragma omp task private(j) depend(inout: s1) depend(in: A, B)
    for(j = 0; j < n; j++)
      #pragma omp atomic
      s1 += A[j] * B[j];

    #pragma omp task private(j) depend(in: s1) depend(inout: A)
    for(j = 0; j < n; j++)
      #pragma omp atomic
      A[j] *= s1;
  }

  #pragma omp parallel default(none) shared(B, C, n, s2)
  #pragma omp single private(i)
  for(i = 0; i < n; i++) {
    // segundo bucle for
    s2 = 0.0;

    #pragma omp task private(j) depend(inout: s2) depend(in: B, C)
    for(int j = 0; j < n; j++)
      #pragma omp atomic
      s2 += B[j] * C[j];

    #pragma omp task private(j) depend(in: s2) depend(inout: C)
    for(int j = 0; j < n; j++)
      #pragma omp atomic
      C[j] *= s2;
  }

  // calculo final
  a = s1 / s2;
#ifdef _DEBUG
  std::cout << "s1: " << s1 << std::endl;
  std::cout << "s2: " << s2 << std::endl;
  std::cout << "a: " << a << std::endl;
#endif
  res = 0;

  for (i = 0; i < n; i++)
    res += a * C[i];

  return res;
}

double OpArrSeq(double *A, int *B, double *C, int n)
{
  int i, j;
  double s1, s2, a, res;

  prefix_sum<double>(A, n); // obtiene el array de suma de prefijos de A

  qs(B, 0, n - 1, 1); // ordena el array B

  scan_left<double>(C, x, n); // acumula los valores de elementos de C mas una constante x = 100

#ifdef _DEBUG
  prnt(A, B, C, n);
#endif

  for (i = 0; i < n; i++)
  { //
    s1 = 0.0;
    for (j = 0; j < n; j++)
      s1 += A[j] * B[j];
    for (j = 0; j < n; j++)
      A[j] *= s1;
  }

  for (i = 0; i < n; i++)
  { //
    s2 = 0.0;
    for (j = 0; j < n; j++)
      s2 += B[j] * C[j];
    for (j = 0; j < n; j++)
      C[j] *= s2;
  }

  a = s1 / s2;
  std::cout << "s1: " << s1 << std::endl;
  std::cout << "s2: " << s2 << std::endl;
  std::cout << "a: " << a << std::endl;

  res = 0;
  for (i = 0; i < n; i++)
    res += a * C[i];

  return res;
}

int main(int argc, char *argv[])
{
#ifdef _DEBUG
  srand(1234);
#else
  srand(time(0));
#endif

  double *A, *C;
  int *B, n, i;
  double res;

  double fMin = 0, fMax = 1;

// Define n
#ifdef _DEBUG
  n = 4;
#else
  n = (int)atoi(argv[1]);
#endif

// Define P & filename
#ifdef _DATA
  P = (int)atoi(argv[2]);
  filename = argv[3];
#endif

  A = new double[n];
  B = new int[n];
  C = new double[n];

  for (i = 0; i < n; i++)
  {
    A[i] = ((double)rand() / RAND_MAX) * (fMax - fMin) + fMin;
    B[i] = rand() % 3 - 1;
    C[i] = ((double)rand() / RAND_MAX) * (fMax - fMin) + fMin;
  }

#ifdef _DEBUG
  double * _A, *_C;
  int * _B;
  _A = new double[n];
  _B = new int[n];
  _C = new double[n];
  std::copy(A, A + n, _A);
  std::copy(B, B + n, _B);
  std::copy(C, C + n, _C);
  res = OpArrSeq(_A, _B, _C, n);
  std::cout << "\tResultadoSeq: " << res << std::endl;

  res = OpArr(A, B, C, n);
  std::cout << "\tResultado: " << res << std::endl;

  delete[] _A;
  delete[] _B;
  delete[] _C;
#else
  auto t0 = std::chrono::steady_clock::now();
  res = OpArr(A, B, C, n);
  std::cout << "\tResultado: " << res << std::endl;
  auto t1 = std::chrono::steady_clock::now();
  auto dtime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
  std::cout << "Tiempo: " << dtime << " ms" << std::endl;

  // Open data.txt and add times comm, comp, total
  std::ofstream myfile;
  myfile.open(filename, std::ios_base::app);
  myfile << std::fixed << std::setprecision(4) << dtime << "\n";
  myfile.close();
#endif

  delete[] A;
  delete[] B;
  delete[] C;

  return 0;
}
