#include <mpi.h>
#include <stdio.h>
#include <iostream>
using namespace std;

// Para el cálculo del tiempo de latencia, enviamos el buffer con un tamaño igual a cero, lo cual también es permitido por las funciones
// de envío no bloqueante.

int main(int argc, char **argv) {
  int size, rank, next, prev, buf[2], tag1=1, tag2=2, N=1024;
  int sbuf[N], rbuf[N];
  double t0,t1;
  MPI_Status stats[2];
  MPI_Init(&argc, &argv);                   // Initialize MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Rank of the processor
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Total number of processors
  MPI_Request reqs[2];

  prev=rank-1;
  next=rank+1;
  if(rank==0) prev=size-1;
  if(rank==(size-1)) next=0;

  t0=MPI_Wtime();
  MPI_Irecv(rbuf,0,MPI_INT,prev,tag1,MPI_COMM_WORLD,&reqs[0]);
  //MPI_Irecv(rbuf,0,MPI_INT,next,tag2,MPI_COMM_WORLD, &reqs[1]);

  //MPI_Isend(sbuf,0,MPI_INT,prev,tag2,MPI_COMM_WORLD, &reqs[2]);
  MPI_Isend(sbuf,0,MPI_INT,next,tag1,MPI_COMM_WORLD, &reqs[1]);

  t1=MPI_Wtime();

  MPI_Waitall(2,reqs,stats);

  if (rank==0) cout<<"Tiempo (s): "<<(t1-t0)*1000<<endl;

  MPI_Finalize();

}
