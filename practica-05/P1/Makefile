CC=g++
CCX=mpic++

CFLAG_DEBUG=-D_DEBUG
CFLAG_DEBUG_PRINT=-D_DEBUG_PRINT
SRC=p1.cpp

P=4
FILE_NAME="data.csv"

e1_debug:
	$(CCX) $(CFLAG_DEBUG) $(SRC) -o $@

e1:
	$(CC) $(SRC) -o $@

run_e1_debug:
	mpirun -np $(P) ./e1_debug $(FILE_NAME)

run_e1:
	mpirun -np $(P) ./e1 $(FILE_NAME)

clean:
	rm e1_debug e1
