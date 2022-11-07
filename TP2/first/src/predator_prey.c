#include "io.h"
#include "predator_prey.h"

int main (int argc, char **argv){

	// get input file size (in chars)
	int input_size;
	get_input_size(&input_size);

	// load input file to buffer
	char input_buffer[input_size];
	load_input(&input_buffer, input_size);

	int GEN_PROC_PREY;
	int GEN_PROC_PREDATOR;
	int GEN_PREDATOR_FOOD;
	int N_GEN, R, C, N;

	load_ecosystem_specs(
		input_buffer,
		&GEN_PROC_PREY,
		&GEN_PROC_PREDATOR,
		&GEN_PREDATOR_FOOD,
		&N_GEN, 
		&R, &C, &N
	);

	board_postion_t board[R][C];
	predator_t predators[R*C];
	prey_t preys[R*C];


	printf("%d %d %d %d %d %d %d\n", GEN_PROC_PREY, GEN_PROC_PREDATOR, GEN_PREDATOR_FOOD, N_GEN, R, C, N);


	// printf("%s\n", input_buffer);
}
