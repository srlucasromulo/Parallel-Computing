#include "predator_prey.h"
#include <stdio.h>


enum directions {NORTH=1, EAST, SOUTH, WEST};

void decrease_gen(subject_t *subjects, int R, int C){

	for (int i = 0; i < R * C; i++){
		if (subjects[i].type == PREDATOR){
			subjects[i].predator.gen_food--;
			subjects[i].predator.gen_proc--;
		}
		if (subjects[i].type == PREY) 
			subjects[i].prey.gen_proc--;
	}
}

int is_board_position(int x, int y, int R, int C){

	if (x >= 0 && x < C)
		if (y >= 0 && y < R)
			return 1;
	return 0;
}

int get_next_position(
	int *current_board,
	int *next_board, 
	subject_t *subjects, 
	int id, int g, int x, int y, int R, int C
){

	// positions around
	int around[4][2] = {{x-1,y}, {x,y+1}, {x+1,y}, {x,y-1}};

	// validate positions in board limits
	int valids[4] = {0,0,0,0};
	int count_valids = 0;
	for (int i = 0; i < 4; i++)
		if (is_board_position(around[i][0], around[i][1], R, C))
			valids[count_valids++] = around[i][0]*C+around[i][1];

	// predators try to catch preys
	if (subjects[id].type == PREDATOR){

		int preys[count_valids];
		int count_preys = 0;

		// check around positions for preys 
		for (int i = 0; i < count_valids; i++)
			if (current_board[valids[i]] >= 0){
				int o = current_board[valids[i]];
				if (subjects[o].type == PREY)
					preys[count_preys++] = valids[i];
			}

		// follow prey
		if (count_preys){
			int pick = (g + x + y) % count_preys;
			return preys[pick];
		}
	}

	// clear fields around
	int clears[count_valids];
	int count_clears = 0;
	for (int i = 0; i < count_valids; i++)
		if (current_board[valids[i]] == EMPTY_FIELD)
			clears[count_clears++] = valids[i];

	// go to clear field
	if (count_clears){
		int pick = (g + x + y) % count_clears;
		return clears[pick];
	}

	// stay
	return x*C+y;
}

void move_subjects(
	int *current_board, 
	int *next_board, 
	subject_t *subjects,
	int g, int R, int C
) {

	// decrease_gen é aqui msm?
	decrease_gen(subjects, R, C);

	for (int x = 0; x < R; x++) {
		for (int y = 0; y < C; y++) {

			int x_y = x*C+y;

			if (current_board[x_y] >= 0){

				int id = current_board[x_y];

				if (subjects[id].type == PREDATOR &&
					subjects[id].predator.gen_food == 0
				){
					// kill predator
				}
				else {
					int next_pos = get_next_position(
						current_board, 
						next_board, 
						subjects,
						id, g, x, y, R, C
					);
					// ...
				}
			}
		}
	}
}
