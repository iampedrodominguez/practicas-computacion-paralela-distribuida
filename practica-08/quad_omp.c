#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <omp.h>

int main(int argc, char *argv[]);
double f(double x);
double cpu_time(void);
void timestamp(void);

/******************************************************************************/

int main(int argc, char *argv[])

/******************************************************************************/
{
  if (argc < 3)
  {
    printf("Too few arguments\n");
    return 1;
  }
  double a = 0.0;
  double b = 10.0;
  double error;
  double exact = 0.49936338107645674464;
  int i;
  int n = atoi(argv[1]);
  int p = atoi(argv[2]);
  double total;
  double wtime;
  double wtime1;
  double wtime2;
  double x;

  timestamp();
  printf("\n");
  printf("QUAD_OMP:\n");
  printf("  Integral de f(x)= 50/( pi * ( 2500 * x * x + 1 ) ).\n");
  printf("A (inicio) = %f\n", a);
  printf("B (fin)= %f\n", b);
  printf("  N = %d\n", n);
  printf("valor exacto = %24.16f\n", exact);
  printf("\n");

  wtime1 = cpu_time();

  total = 0.0;

#pragma omp parallel for private(x) reduction(+ : total) num_threads(p) schedule(static)
    for (i = 0; i < n; i++)
    {
      x = ((n - i - 1) * a + (i)*b) / (n - 1);
      total += f(x);
    }

  wtime2 = cpu_time();

  total = (b - a) * total / (double)n;
  error = fabs(total - exact);
  wtime = wtime2 - wtime1;

  printf("\n");
  printf("  Estimate = %24.16f\n", total);
  printf("  Error    = %e\n", error);
  printf("  Time     = %f\n", wtime);
  /*
    Terminate.
  */
  printf("\n");
  printf("QUAD_OMP:\n");
  printf("  Normal end of execution.\n");
  printf("\n");
  timestamp();

  return 0;
}
double f(double x)
{
  double pi = 3.141592653589793;
  double value;

  value = 50.0 / (pi * (2500.0 * x * x + 1.0));

  return value;
}
double cpu_time(void)
{
  double value;

  value = (double)clock() / (double)CLOCKS_PER_SEC;

  return value;
}
void timestamp(void)
{
#define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;

  now = time(NULL);
  tm = localtime(&now);

  strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm);

  printf("%s\n", time_buffer);

  return;
#undef TIME_SIZE
}
