# ### A simple basic-purpose makefile ###

## Works w/ multiple C source files, but
## BEWARE: It only creates ONE binary

### How to USE
### Change the binary name (1)

# Executable
## Change "exec" to the bin name you want (1)
EXEC = exec

# Binary
BIN = ./bin/$(EXEC)

# Source files
SRC = $(wildcard ./src/*.c)

# Objects
OBJ = $(subst src,obj,$(subst .c,.o,$(SRC)))

# Compiler
C_CC = gcc
CPP_CC = gpp
MPI_CC = mpicc
CC = $(MPI_CC)

# Compiler flags
CFLAGS = -lm -Wall -fopenmp
CCFLAGS = $(CFLAGS)


# Compiling and linking
#
.PHONY: all objFolder binFolder
all: objFolder binFolder $(BIN)

$(BIN): $(OBJ)
	@ $(CC) $(OBJ) -o $@ $(CCFLAGS)

./obj/%.o: ./src/%.c ./src/*.h
	@ $(CC)  -c $< -o $@ $(CCFLAGS)

objFolder:
	@ if [ ! -d ./obj ]; then \
		mkdir -p obj ; \
	fi

binFolder:
	@ if [ ! -d ./bin ]; then \
		mkdir -p bin ; \
	fi

.PHONY: clean run gprof
clean:
	@ rm -rf ./obj/* ./obj
	@ rm -rf ./bin/* ./bin
	@ rm -f time output

OMP_THREADS=2
MPI_THREADS=8
run: all
	@ export OMP_NUM_THREADS=$(OMP_THREADS)
	@ mpirun -n $(MPI_THREADS) --machinefile machinefile $(BIN)
