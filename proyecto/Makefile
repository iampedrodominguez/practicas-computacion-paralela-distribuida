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
N = 25
P = 1
T = 2

%.data: %.cpp
	@echo -e "SERIAL::Compiling $@ DATA"
	@$(CXX) $(CCFLAGS) $(DATAFLAGS) -o $@ $<

%.test: %.cpp
	@echo -e "SERIAL::Compiling $@ TEST"
	@$(CXX) $(CCFLAGS) $(TESTFLAGS) -o $@ $<

omp.%.data: %.cpp
	@echo -e "OMP::Compiling $@ DATA"
	@$(CXX) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $@ $<

omp.%.test: %.cpp
	@echo -e "OMP::Compiling $@ TEST"
	@$(CXX) $(LDFLAGS) $(CCFLAGS) $(TESTFLAGS) -o $@ $<

mpi.%.data: %.cpp
	@$(CC) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $@ $<

mpi.%.test: %.cpp
	@$(CC) $(LDFLAGS) $(CCFLAGS) $(TESTFLAGS) -o $@ $<

mpi.%.run: %
	@$(MKDIR) $(DATA_DIR)
	@mpirun -np $(P) ./$(TARGET) $(DATA_DIR)/$(FILE) $(N) $(T)

omp.%.run: %
	@$(MKDIR) $(DATA_DIR)
	@echo -e "OMP::Running $<"
	@./$< $(N)

%.run: %
	@$(MKDIR) $(DATA_DIR)
	@echo -e "Running $<"
	@./$< $(N)

clean:
	@rm -rf $(TARGET)

clean_data:
	