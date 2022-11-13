#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <string>

using namespace std;
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
  double a_process;
  double b_process;
  double error;
  double exact = 0.49936338107645674464;
  int i;
  int n = 10000000;
  n = atoi(argv[1]);
  string filename = argv[2];
  double total;
  double wtime;
  double wtime1;
  double wtime2;
  double x;
  double my_total;
  int id_process;
  int id;                // proceso
  int number_of_process; // total de procesos

  MPI_Status s;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &number_of_process);

  if (id == 0)
  {
    wtime = MPI_Wtime();
    //timestamp();
  }
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD); // wnviando n a todos los procesos

  if (id == 0)
  {
    for (id_process = 1; id_process <= number_of_process - 1; id_process++)
    {
      // enviando las fronteras a todos los procesos restantes distintos del maestro
      a_process = ((number_of_process - id_process) * a + (id_process - 1) * b) / (number_of_process - id_process) * 1.0;
      MPI_Send(&a_process, 1, MPI_DOUBLE, id_process, 1, MPI_COMM_WORLD);
      b_process = ((number_of_process - id_process - 1) * a + id_process * b) / (number_of_process - 1) * 1.0;
      MPI_Send(&b_process, 1, MPI_DOUBLE, id_process, 2, MPI_COMM_WORLD);
    }
    total = 0.0;
    my_total = 0.0;
  }
  else
  {
    // cada proceso recive las fronteras dada por el maestro
    MPI_Recv(&a_process, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &s);
    MPI_Recv(&b_process, 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &s);

    my_total = 0.0;
    for (i = 1; i <= n; i++)
    {
      x = ((double)(n - i) * a_process + (double)(i - 1) * b_process) / (double)(n - 1);
      my_total = my_total + f(x);
    }

    my_total = (b_process - a_process) * my_total / (double)(n);
  }
  MPI_Reduce(&my_total, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  if (id == 0)
  {
    error = fabs(total - exact);
    wtime = MPI_Wtime() - wtime;

    printf("\n");
    //printf("  Estimate = %24.16f\n", total);
    printf("  Error    = %e\n", error);
    printf("  Time     = %f\n", wtime);

    // Save data to file
    FILE *fp;
    fp = fopen(filename.c_str(), "a");
    fprintf(fp, "mpi,%d,%d,%f,%f\n", n, number_of_process, wtime, error);
    fclose(fp);
  }
  /*
    Terminate.
  */
  MPI_Finalize();
  if (id == 0)
  {
    /*printf("\n");
    printf("QUAD_SERIAL:\n");
    printf("  Normal end of execution.\n");
    printf("\n");
    timestamp();*/
  }
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