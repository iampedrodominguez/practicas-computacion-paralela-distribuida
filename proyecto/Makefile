CC := mpic++
CXX := g++

LDFLAGS := -fopenmp -lpthread
CCFLAGS := -std=c++2a

TESTFLAGS := -D_TEST
DATAFLAGS := -D_DATA

DATA_DIR := ./Data
MKDIR := mkdir -p

TARGET := $(notdir $(CURDIR))
FILE = data.csv
N = 30
P = 1
T = 2

%.data: %.cpp
	@$(CXX) $(CCFLAGS) $(DATAFLAGS) -o $@ $<

%.test: %.cpp
	@echo -e "Compiling $@"
	@$(CXX) $(CCFLAGS) $(TESTFLAGS) -o $@ $<

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

%.run: %
	@$(MKDIR) $(DATA_DIR)
	@echo -e "Running $<"
	@./$< $(N)

clean:
	@rm -rf $(TARGET)

clean_data:
	