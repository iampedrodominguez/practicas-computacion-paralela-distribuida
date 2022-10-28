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

    Por probabilidades, la probabilidad de éxito en cada iteración sería
        1/100 * 1/100 * 1/100 * 1/100 = 1/100 000 000 = 10^-8
    Por lo que, en 1000 intentos, la probabilidad de éxito sería
        (10^-8)^1000 = 10^-8000

    Para debuggear, se utiliza un máximo valor de 2 (solo dos opciones, 1 y 2) y un máximo de 10 intentos.
    Para este caso, la probabilidad de éxito: 1/16, y de fracaso: 15/16.
    1 o 2, 16 posibilidades, 1 forma de ganar
    P(acertar) = 1/16 = 0.0625
    P(fallar) = 15/16

    P(A/B) = P(A int B) / P(B)
    P(A/B) = P(A)*P(B/A)/P(B)
    - P(Acertar en el 2do / fallar en el 1ero) = P(acertar en el 2do y fallar en el 1ero) / P(fallar en el 1ero) = 
    15/240 / 15/16 = 16/240 = 1/15 = 0.066666666666666666667

    
    1ero                                2do
    15 posibilidades que falla          16 posibilidades para cada arreglo anterior -> 240 en total

    validos son 15
    - P(no acertar en el 2do dado que falle en el 1ero) = 1 - 1/15 = 14/15 = 0.9333333333333333333333

    
    
    - P(Acertar en el 3ero / falle en los dos primeros) = P(acertar en el 3ero y fallar en los dos primeros) / P(fallar en los dos primeros) = 
    225/15*15*16 / 14/15 = 1/16 / 14/15 = 15/ (14*16) = 15 / 224 = 0.066964286

    1ero        2do         3ero
    15 falla    15 falla    16 posibilidades para cada uno de los 225 -> 15*15*16 en total.



    P(Acertar en el 10mo fallando los 9 primeros)
    1ero        2do         3ero        4to         ...     9no         10mo
    15 falla    15 falla    15 falla    15 falla    ...     15 falla    Acierta

    P = 15 a la 9 / 15 a la 15 = 15 a la -6
    
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
        fprintf(fp, "%d\n", continueComputing);
    }

    MPI_Finalize();

    return 0;
}
