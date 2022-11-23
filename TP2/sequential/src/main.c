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

	// create subjects (predator/prey/obstacles) list
	subject_t subjects[specs[R]*specs[C]];
	empty_subjects_list((subject_t*)&subjects, specs[R]*specs[C]);

	// load board and subjects 
	load_ecosystem_disposition(
		input_buffer,
		specs,
		(subject_t*)&subjects
	);

	print_subjects_list(subjects, specs[N]);

	for (int g = 0; g < specs[N_GEN]; g++){

		// copy current subjects list to a new one
		subject_t next_subjects[specs[R]*specs[C]];
		copy_subjects_list((subject_t*)&next_subjects, subjects, specs[R]*specs[C]);

		move_preys(
			subjects, (subject_t*)&next_subjects,
			g, specs[R], specs[C], &specs[N], 
			specs[GEN_PROC_PREY]
		);

		copy_subjects_list((subject_t*)&subjects, next_subjects, specs[R]*specs[C]);

		move_predators(
			subjects, (subject_t*)&next_subjects,
			g, specs[R], specs[C], &specs[N], 
			specs[GEN_PROC_PREY], specs[GEN_PREDATOR_FOOD]
		);

		copy_subjects_list((subject_t*)&subjects, next_subjects, specs[R]*specs[C]);

		solve_conflicts(subjects, (subject_t*)&next_subjects, &specs[N]);

		// DBG - print board
		char board[specs[R]][specs[C]];
		for (int i = 0; i < specs[R]; i++)
			for (int j = 0; j < specs[C]; j++)
				board[i][j] = NONE;

		int n_ = 0;
		for (int i = 0; n_ < specs[N]; i++){
			if (subjects[i].type != NONE){
				board[subjects[i].x][subjects[i].y] = subjects[i].type;
				n_++;
			}
		}

		printf("GEN %d\n", g+1);
		printf("-------\n");
		for (int i = 0; i < specs[R]; i++){
			printf("|");
			for (int j = 0; j < specs[C]; j++){
				printf("%c", board[i][j]);
			}
			printf("|\n");
		}
		printf("-------\n");


	}

	// save_output((int*)current_board, subjects, specs);
}
