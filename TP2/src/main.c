#include "io.h"
#include "predator_prey.h"
#include <stdio.h>


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

	for (int g = 0; g < specs[N_GEN]; g++){

		subject_t next_subjects[specs[R]*specs[C]];
		copy_subjects_list(next_subjects, subjects, specs[R]*specs[C]);

		move_preys(
			subjects, (subject_t*)&next_subjects,
			g, specs[R], specs[C], &specs[N], 
			specs[GEN_PROC_PREY]
		);

		copy_subjects_list(subjects, next_subjects, specs[R]*specs[C]);

		move_predators(
			subjects, (subject_t*)&next_subjects,
			g, specs[R], specs[C], &specs[N], 
			specs[GEN_PROC_PREY], specs[GEN_PREDATOR_FOOD]
		);

		solve_conflicts((subject_t*)&next_subjects, specs[R]*specs[C], &specs[N]);

		copy_subjects_list(subjects, next_subjects, specs[R]*specs[C]);
	}
	save_output(subjects, specs);
}
