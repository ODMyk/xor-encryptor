SRC_DIR = src
OUT_DIR = dist

BASIC_SRC = $(SRC_DIR)/basic.cpp
OPENMP_SRC = $(SRC_DIR)/openmp.cpp
MPI_SRC = $(SRC_DIR)/mpi.cpp

BASIC_BIN = $(OUT_DIR)/basic
OPENMP_BIN = $(OUT_DIR)/openmp
MPI_BIN = $(OUT_DIR)/mpi

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

LIBOMP_PREFIX = /opt/homebrew/opt/libomp
OPENMP_FLAG = -Xpreprocessor -fopenmp -I$(LIBOMP_PREFIX)/include -L$(LIBOMP_PREFIX)/lib -lomp

MPICXX = mpic++

all: $(BASIC_BIN)

basic: $(BASIC_BIN)
openmp: $(OPENMP_BIN)
mpi: $(MPI_BIN)

$(BASIC_BIN): $(BASIC_SRC)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@
	@echo "✅ Built: $@"

$(OPENMP_BIN): $(OPENMP_SRC)
	@mkdir -p $(OUT_DIR)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAG) $< -o $@
	@echo "✅ Built: $@"

$(MPI_BIN): $(MPI_SRC)
	@mkdir -p $(OUT_DIR)
	$(MPICXX) $(CXXFLAGS) $< -o $@
	@echo "✅ Built: $@"

clean:
	@rm -rf $(OUT_DIR)
	@echo "🧹 Cleaned $(OUT_DIR)"

.PHONY: all basic openmp mpi clean
