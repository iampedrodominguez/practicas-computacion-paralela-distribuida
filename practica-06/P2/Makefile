CC=g++
CCX=mpic++

CFLAG_DEBUG=-D_DEBUG
SRC=p2.cpp
SRC_DATA=generate_random_data.cpp

P=4
N=32
FILE_NAME=data32.txt
EXEC_FILE=p2

e2_debug:
	$(CC) -fopenmp $(CFLAG_DEBUG) $(SRC) -o $(EXEC_FILE)

e2:
	$(CC) -fopenmp $(SRC) -o $(EXEC_FILE)

e2_data:
	$(CC) $(SRC_DATA) -o $(EXEC_FILE)

run_e2_debug:
	./$(EXEC_FILE) $(FILE_NAME) $(P)

run_e2:
	./$(EXEC_FILE) $(FILE_NAME) $(P)

run_e2_data:
	./$(EXEC_FILE) $(N) 

clean:
	rm $(EXEC_FILE)
