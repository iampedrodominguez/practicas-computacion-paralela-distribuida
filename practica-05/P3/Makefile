CC := mpic++
CXX := clang++

LDFLAGS := -fopenmp -lpthread
CCFLAGS := -std=c++20

TESTFLAGS := -D_TEST
DATAFLAGS := -D_DATA

DATA_DIR := ./data
MKDIR := mkdir -p

TARGET := $(notdir $(CURDIR))
FILE = data.csv
N = 1000
P = 1
T = 2

%.mpi.data: %.cpp
	@$(CC) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $(TARGET) $<

%.mpi.test: %.cpp
	@$(CC) $(LDFLAGS) $(CCFLAGS) $(TESTFLAGS) -o $(TARGET) $<

mpi.run:
	@$(MKDIR) $(DATA_DIR)
	@mpirun -np $(P) ./$(TARGET) $(DATA_DIR)/$(FILE) $(N) $(T)

cc.run:
	@$(MKDIR) $(DATA_DIR)
	@$(CXX) ./$(TARGET) $(DATA_DIR)/$(FILE) $(N) $(T)

clean:
	@rm -rf $(TARGET)

clean_data:
	@rm -rf ./data/