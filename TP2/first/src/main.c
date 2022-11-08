#include "io.h"
#include "predator_prey.h"
#include <stdio.h>


int main (int argc, char **argv){

	// get input file size (in chars)
	int input_size;
	get_input_size(&input_size);

	// load input file to buffer
	char input_buffer[input_size];
	load_input(&input_buffer, input_size);

	int specs[7];
	load_ecosystem_specs(input_buffer, &specs);

	int current_board[specs[R] * specs[C]];
	empty_board(&current_board, specs[R], specs[C]);

	subject_t subjects[specs[R] * specs[C]];
	empty_subjects_list(&subjects, specs[R], specs[C]);

	load_ecosystem_disposition(
		input_buffer,
		specs,
		&current_board,
		&subjects
	);

	print_board(current_board, specs[R], specs[C]);
	print_subjects_list(subjects, specs[R], specs[C]);


	printf("%d %d %d %d %d %d %d\n", 
		specs[GEN_PROC_PREY], 
		specs[GEN_PROC_PREDATOR],
	 	specs[GEN_PREDATOR_FOOD], 
	 	specs[N_GEN], 
	 	specs[R], specs[C], specs[N]);
}
