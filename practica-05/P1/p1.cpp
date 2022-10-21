#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int argc, char **argv)
{
    int size, rank;
    int data[5];
    int localdata;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(123456);

    // inicializacion de data[0] en proceso 0
    if (rank == 0)
    {
        for (int i = 0; i < size; i++)
            globaldata[i] = 0;
        globaldata[0] = rand() % 100 + 1;
        printf("Data en 0: %d\n", globaldata[0]);
    }

    // Comunicacion (scatter)
    MPI_Scatter(globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank != 0)
    {
        localdata = rand() % 100 + 1;
    }

    // Comunicacion (gather)
    MPI_Gather(&localdata, 1, MPI_INT, globaldata, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // resultado en proceso 0
    if (rank == 0)
    {
        printf("Resultado: %d %d %d %d %d\n", globaldata[0], globaldata[1], globaldata[2], globaldata[3], globaldata[4]);
    }

    MPI_Finalize();
    return 0;
}
