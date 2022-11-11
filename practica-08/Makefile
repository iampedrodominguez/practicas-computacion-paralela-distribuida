CC=g++
CCX=mpic++

SRC_SERIAL=quad_serial.c
SRC_OMP=quad_omp.c
SRC_MPI=quad_mpi.c

N=10000000
P=2
FILE_NAME="data.csv"

e_serial:
	$(CC) $(SRC_SERIAL) -o ./e1.out

e_omp:
	$(CC) -fopenmp $(SRC_OMP) -o ./e2.out

e_mpi:
	$(CCX) $(SRC_MPI) -o ./e3.out

run_e_serial:
	./e1.out $(N)

run_e_omp:
	./e2.out $(N) $(P)

run_e_mpi:
	mpirun -np $(P) ./e3.out $(N)

clean:
	rm e1.out e2.out e3.out
