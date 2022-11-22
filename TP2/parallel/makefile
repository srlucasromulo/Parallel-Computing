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
CC = $(C_CC)

# Compiler flags
CFLAGS = -lm -Wall -pg -fopenmp
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
	mkdir -p obj

binFolder:
	mkdir -p bin

.PHONY: clean run gprof profile
clean:
	@ rm -rf ./obj/* ./obj
	@ rm -rf ./bin/* ./bin
	@ rm -f profile output gmon.out
	@ rm -f ./time/time*

run: all
	@ $(BIN) > ./time/time_elapsed

gprof: run
	@ gprof -b $(BIN) gmon.out > profile

profile: run
	@ cd time && python script.py
