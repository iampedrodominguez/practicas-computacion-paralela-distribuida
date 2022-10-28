CC := mpic++
CXX := g++

LDFLAGS := -fopenmp -lpthread
CCFLAGS := -std=c++20

TESTFLAGS := -D_TEST
DATAFLAGS := -D_DATA

DATA_DIR := ./data
MKDIR := mkdir -p

TARGET := $(notdir $(CURDIR))
FILE = data.csv
N = 10
P = 1
T = 2

%.omp.data: %.cpp
	@$(CXX) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $(TARGET) $<

%.omp.test: %.cpp
	@$(CXX) $(LDFLAGS) $(CCFLAGS) $(TESTFLAGS) -o $(TARGET) $<

%.mpi.data: %.cpp
	@$(CC) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $(TARGET) $<

%.mpi.test: %.cpp
	@$(CC) $(LDFLAGS) $(CCFLAGS) $(TESTFLAGS) -o $(TARGET) $<

mpi.run:
	@$(MKDIR) $(DATA_DIR)
	@mpirun -np $(P) ./$(TARGET) $(DATA_DIR)/$(FILE) $(N) $(T)

omp.run:
	@$(MKDIR) $(DATA_DIR)
	@./$(TARGET) $(DATA_DIR)/$(FILE) $(N) $(T)

clean:
	@rm -rf $(TARGET)

clean_data:
	@rm -rf ./data/
