#include <mpi.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

/*
    El proceso 0 genera un random que será el objetivo del juego y lo envía a los 4 procesos con un broadcast.

    Se usará la estrategia centralizada para la generación de los números random, ya que, si bien es de bajo
    performance, no crea correlaciones como otros métodos. Entonces, el proceso 0 generará los cuatro números
    random y los enviará a los procesos con un Scatter. Cada proceso recibe el número y lo compara con el objetivo.

    Finalmente, se utiliza un Reduce con la operación AND para verificar que los cuatro procesos hayan encontrado
    el número objetivo. La variable continueComputing cambiará a 0 si ya se encuentra una solución para no seguir
    buscando.

    Por probabilidades, correspondería a una probabilidad de éxito de 1/100^4, de fracaso 1-(1/100^4), y un 
    máximo de 1000 intentos. Esto consiste en una probabilidad binomial acumulativa P(X >= 1) = 1 - P(X = 0) = 
    1 - 1000C0 * (1/100^4)^0 * (1 - 1/100^4)^1000 = 1 - 1 * 1 * (99 999 999/100 000 000)^1000 
    = 9.9999500501661665858295775277568e-6 = 0.0000099999500501661665858295775277568 = 0.00001 = 0.001%.
    Es decir, es casi imposible que Eduardo invite la cena.

    Para probar el código, se utiliza un máximo valor de 2 (solo dos opciones, 1 y 2) y un máximo de 10 intentos.
    Para este caso, la probabilidad de éxito: 1/16, y de fracaso: 15/16. Consiste de una probabilidad binomial 
    acumulativa P(X >= 1) o 1 - P(X = 0) = 1 - 10C0 * 1/16^(0) * (15/16)^(10-0) = 1 - 0.52446047504872694844380021095276 
    = 0.47553952495127305155619978904724 = 47.55%, lo que se demuestra en la gráfica.

    Como se observa en la gráfica de 47.55% de probabilidad, los experimentos se ajustan a este valor, lo cual no se
    observa en la gráfica de 0.001% de probabilidad ya que es muy baja. En conclusión, bajo las condiciones del
    problema, Eduardo no invitará la cena.
*/

int main(int argc, char **argv)
{
    int size, rank;
    int globaldata[4];
    int goal;
    int localdata;
    int continueComputing = 1;
    string filename = "data.csv";

#ifdef _DEBUG
    int times = 10;
    int val_max = 2;
#else
    int times = 1000;
    int val_max = 100;
#endif
    
    int globalcomparation = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int localcomparation = 0;

    srand(time(NULL));

    if(rank == 0)
    {
        if(argc > 1)
            filename = argv[1];
        goal = rand() % val_max + 1;
        printf("Goal: %d\n", goal);
    }

    MPI_Bcast(&goal, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    for (int i = 0; i < times && continueComputing == 1; i++)
    {
        if(rank == 0)
        {
            for (int j = 0; j < size; j++)
                globaldata[j] = rand() % val_max + 1;
#ifdef _DEBUG_PRINT
            printf("Iteration %d: ", i);
            printf(" Global data %d %d %d %d\t", globaldata[0], globaldata[1], globaldata[2], globaldata[3]);
#endif
        }

        // Comunicación Scatter
        MPI_Scatter(globaldata, 1, MPI_INT, &localdata, 1, MPI_INT, 0, MPI_COMM_WORLD);

        localcomparation = goal == localdata;

        // Comunicacion Reduce
        MPI_Reduce(&localcomparation, &globalcomparation, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);

        // resultado en proceso 0
        if (rank == 0)
        {
#ifdef _DEBUG_PRINT
            printf("Result: %d\n", globalcomparation);
#endif
            if(globalcomparation)
                continueComputing = 0;    
        }

        MPI_Bcast(&continueComputing, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if(rank == 0){
        if(!continueComputing)
            printf("Eduardo invita\n");
        else
            printf("No hay almuerzo\n");
        
        //Print to file
        FILE *fp;
        fp = fopen(filename.c_str(), "a");
        fprintf(fp, "%d\n", !continueComputing);
    }

    MPI_Finalize();

    return 0;
}
