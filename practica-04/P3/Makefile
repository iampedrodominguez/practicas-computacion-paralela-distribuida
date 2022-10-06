CC=g++
CCX=mpic++

CFLAG_DEBUG=-D_DEBUG
CFLAG_DATA=-D_DATA
CFLAG_DEBUG_SEQ=-D_SEQ
SRC=p3.cpp

P=4
FILE_NAME="data.csv"
N=512

e3_debug:
	$(CC) -fopenmp $(CFLAG_DEBUG) $(SRC) -o $@

e3_data:
	$(CC) -fopenmp $(CFLAG_DATA) $(SRC) -o $@

e3_debug_seq:
	$(CC) -fopenmp $(CFLAG_DEBUG) $(CFLAG_DEBUG_SEQ) $(SRC) -o $@

run_e3_debug:
	./e3_debug

run_e3_data:
	./e3_data $(N) $(P) $(FILE_NAME)

run_e3_debug_seq:
	./e3_debug_seq

clean:
	rm e3_debug e3_data e3_debug_seq
