#include "io.h"
#include "predator_prey.h"


int main (int argc, char **argv){

	// get input file size (in chars)
	int input_size;
	get_input_size((int*)&input_size);

	// load input file to buffer
	char input_buffer[input_size];
	load_input((char*)&input_buffer, input_size);

	// load specs: board size, n_gen, proc_gen, ...
	int specs[7];
	load_ecosystem_specs(input_buffer, (int*)&specs);

	// create board
	int current_board[specs[R]][specs[C]];
	empty_board((int*)&current_board, specs[R], specs[C]);

	// create subjects (predator/prey) list
	subject_t subjects[specs[R] * specs[C]];
	empty_subjects_list((subject_t*)&subjects, specs[R], specs[C]);

	// load board and subjects 
	load_ecosystem_disposition(
		input_buffer,
		specs,
		(int*)&current_board,
		(subject_t*)&subjects
	);

	for (int g = 0; g < specs[N_GEN]; g++){

		// copy current board to a new one
		int next_board[specs[R] * specs[C]];
		copy_board((int*)&next_board, (int*)current_board, specs[R]*specs[C]);

		move_preys(
			(int*)current_board, 
			(int*)&next_board, 
			(subject_t*)&subjects,
			g, specs[R], specs[C], &specs[N],
			specs[GEN_PROC_PREY]
		);

		// modified board is now the current board
		copy_board((int*)&current_board, (int*)next_board, specs[R]*specs[C]);

		move_predators(
			(int*)current_board, 
			(int*)&next_board, 
			(subject_t*)&subjects,
			g, specs[R], specs[C], &specs[N],
			specs[GEN_PROC_PREDATOR], specs[GEN_PREDATOR_FOOD]
		);

		// modified board is now the current board
		copy_board((int*)&current_board, (int*)next_board, specs[R]*specs[C]);
	}

	save_output((int*)current_board, subjects, specs);
}
