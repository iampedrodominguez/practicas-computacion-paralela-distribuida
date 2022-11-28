CC := mpic++
CXX := g++

LDFLAGS := -fopenmp -lpthread
CCFLAGS := -std=c++2a

TESTFLAGS := -D_TEST
DATAFLAGS := -D_DATA

DATA_DIR := ./Data
MKDIR := mkdir -p

# NOTE: Variables for experiments

TARGET := $(notdir $(CURDIR))
FILE = data.csv
PFILE= points_random.txt
N = 28
P = 1
T = 2

# NOTE: Global variables

CBLUE 		:= 
CBOLD 		:= 
CEND 		:= 
CGREEN 		:= 
CRED 		:= 
operation 	= 'Waiting...'

build.%: %.cpp
	$(eval operation := Compiling)
	@echo -e "$(CBOLD)SERIAL$(CBLUE)::$(CEND)$(operation) $< $(CGREEN)$(CBOLD)==>$(CEND) $@"
	@echo -e "$(CBOLD)SERIAL$(CBLUE)::$(CEND)FLAG$(CBLUE)::$(CEND)$(CBOLD)DATA$(CEND)"
	@$(CXX) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $@ $<

build.%.omp: %.cpp
	$(eval operation := Compiling)
	@echo -e "$(CBOLD)OMP$(CBLUE)::$(CEND)$(operation) $< $(CGREEN)$(CBOLD)==>$(CEND) $@"
	@echo -e "$(CBOLD)OMP$(CBLUE)::$(CEND)FLAG$(CBLUE)::$(CEND)$(CBOLD)DATA$(CEND)"
	@$(CXX) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $@ $<

build.%.mpi: %.cpp
	$(eval operation := Compiling)
	@echo -e "$(CBOLD)MPI$(CBLUE)::$(CEND)$(operation) $< $(CGREEN)$(CBOLD)==>$(CEND) $@"
	@echo -e "$(CBOLD)MPI$(CBLUE)::$(CEND)FLAG$(CBLUE)::$(CEND)$(CBOLD)DATA$(CEND)"
	@$(CC) $(LDFLAGS) $(CCFLAGS) $(DATAFLAGS) -o $@ $<

run.%: build.%
	$(eval operation := Running)
	@echo -e "$(CBOLD)SERIAL$(CBLUE)::$(CEND)$(operation) $<"
	@$(MKDIR) $(DATA_DIR)
	@./$< $(DATA_DIR)/$(PFILE) $(DATA_DIR)/$(TFILE)

run.%.omp: build.%.omp
	$(eval operation := Running)
	@echo -e "$(CBOLD)OMP$(CBLUE)::$(CEND)$(operation) $<"
	@$(MKDIR) $(DATA_DIR)
	@./$< $(T) $(DATA_DIR)/$(PFILE) $(DATA_DIR)/$(TFILE)

run.%.mpi: build.%.mpi
	$(eval operation := Running)
	@echo -e "$(CBOLD)MPI$(CBLUE)::$(CEND)$(operation) $<"
	@$(MKDIR) $(DATA_DIR)
	@mpirun -np $(P) ./$(TARGET) $(DATA_DIR)/$(FILE) $(N) $(T)

run.%.data: build.%
	$(eval operation := Running)
	@echo -e "$(CBOLD)SERIAL$(CBLUE)::$(CEND)$(operation) $<"
	@$(MKDIR) $(DATA_DIR)
	@./$< $(N)

clean:
	$(eval operation := Cleanning...)
	@echo -e "$(CBOLD)$(CBLUE)::$(CEND)$(operation)"
	@rm -rf *.data *.test $(TARGET)

clean.data:
	$(eval operation := Cleanning data...)
	@echo -e "$(CBOLD)$(CBLUE)::$(CEND)$(operation)"