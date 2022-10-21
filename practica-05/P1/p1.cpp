#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

/*
    El proceso 0 genera los 4 números random (si cada proceso genera un random a partir de alguna 
    semilla como time(NULL) + rank estarían correlacionados, por lo que se uso este método que no es 
    eficiente pero sí eficaz), los envía a cada proceso y compara con goal. El resultado se obtiene de
    un reduce.

    Se obtiene 0 ya que la probabilidad de éxito es muy baja (100/100^5 = 1/100^3)
*/

int main(int argc, char **argv)
{
    int size, rank;
    int globaldata[4];
    int goal;
    int localdata;
    int times = 1000;
    int ended = 0;
    int val_max = 100;
    
    int globalcomparation = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int localcomparation = 0;

    srand(time(NULL));

    if(rank == 0)
    {
        goal = rand() % val_max + 1;
        printf("Goal: %d\n", goal);
    }

    MPI_Bcast(&goal, 1, MPI_INT, 0, MPI_COMM_WORLD);

    //MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < times; i++)
    {
        if(rank == 0)
        {
            for (int j = 0; j < size; j++)
            {
                globaldata[j] = rand() % val_max + 1;
                //printf("Globaldata[%d]: %d\n", j, globaldata[j]);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);

        // Comunicación Scatter
        MPI_Scatter(globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

        localcomparation = goal == localdata;

        // Comunicacion (reduce)
        MPI_Reduce(&localcomparation, &globalcomparation, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);

        // resultado en proceso 0
        if (rank == 0)
        {
            //printf("Resultado: %d\n\n", globalcomparation);
            if(globalcomparation){
                printf("Valores %d %d %d %d\n", globaldata[0], globaldata[1], globaldata[2], globaldata[3]);
                ended = 1;
            }
        }

    }

    if(rank == 0){
        if(ended)
            printf("Eduardo invita\n");
        else
            printf("No hay almuerzo\n");
    }

    MPI_Finalize();

    return 0;
}
