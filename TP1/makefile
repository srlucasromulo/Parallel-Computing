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
CFLAGS = -lm -Wall
CCFLAGS = $(CFLAGS)


# Compiling and linking
#
.PHONY: all objFolder binFolder
all: objFolder binFolder $(BIN)

$(BIN): $(OBJ)
	@ echo 'Building binary $@ using $(CC)...'
	@ $(CC) $(OBJ) -o $@ $(CCFLAGS)
	@ echo 'Finished building $@.'

./obj/%.o: ./src/%.c ./src/*.h
	@ echo 'Building $@ from $<...'
	@ $(CC)  -c $< -o $@ $(CCFLAGS)
	@ echo 'ok'

objFolder:
	@ if [ ! -d ./obj ]; then \
		echo 'Creating objects folder...' ; \
		mkdir -p obj ; \
		echo 'ok'; \
	fi

binFolder:
	@ if [ ! -d ./bin ]; then \
		echo 'Creating binary folder...' ; \
		mkdir -p bin ; \
		echo 'ok'; \
	fi

.PHONY: clean run
clean:
	@ echo 'Cleanning up...'
	@ rm -rf ./obj/* ./obj
	@ rm -rf ./bin/* ./bin
	@ rm output time
	@ echo 'ok'

run: all
	@ mpirun -n 8 --machinefile machinefile $(BIN)
