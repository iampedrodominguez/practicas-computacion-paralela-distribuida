CC=g++
CCX=mpic++

CFLAG_TEST=-D_TEST
CFLAG_DATA=-D_DATA
SRC=P32.cpp

P=4
FILE_NAME="data.txt"
N=512

e3_test:
	$(CCX) $(CFLAG_TEST) $(SRC) -o $@

e3_data:
	$(CCX) $(CFLAG_DATA) $(SRC) -o $@

run_e3_test:
	mpirun -np 2 ./e3_test

run_e3_data:
	mpirun -np $(P) ./e3_data $(FILE_NAME) $(N)

clean:
	rm e3_test e3_data
