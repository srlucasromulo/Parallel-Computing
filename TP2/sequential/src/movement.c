#include "predator_prey.h"
#include <stdio.h>


enum directions {NORTH=1, EAST, SOUTH, WEST};


int is_board_position(int x, int y, int R, int C){
	if (x >= 0 && x < C)
		if (y >= 0 && y < R)
			return 1;
	return 0;
}

void get_valid_positions_around(
	int *current_board,
	int x, int y,
	int R, int C,
	int *positions, int *count
){
	// positions around
	int around[4][2] = {{x-1,y}, {x,y+1}, {x+1,y}, {x,y-1}};

	for (int i = 0; i < 4; i++)
		if (is_board_position(around[i][0], around[i][1], R, C))
			positions[(*count)++] = around[i][0]*C+around[i][1];
}

void get_empty_positions_around(
	int *current_board,
	int *valids, int count_valids,
	int *empty, int *count_empty
){
	for (int i = 0; i < count_valids; i++)
		if (current_board[valids[i]] == EMPTY_FIELD)
			empty[(*count_empty)++] = valids[i];
}

void get_prey_positions_around(
	int *current_board,
	subject_t* subjects,
	int *valids, int count_valids,
	int *prey, int *count_preys
){
	for (int i = 0; i < count_valids; i++){
		int other_id = current_board[valids[i]];
		if (subjects[other_id].type == PREY)
			prey[(*count_preys)++] = valids[i];
	}
}

int solve_conflict(subject_t *list, int a, int b){

	if (list[a].type != list[b].type)
		return list[a].type == PREDATOR ? a : b;

	if (list[a].type == PREDATOR) {
		if (list[a].predator.gen_proc == list[b].predator.gen_proc)
			return list[a].predator.gen_food > list[b].predator.gen_food ? a : b;
		else
			return list[a].predator.gen_proc < list[b].predator.gen_proc ? a : b;
	}
	
	if (list[a].type == PREY)
		return list[a].prey.gen_proc < list[b].prey.gen_proc ? a : b;

	return a;
}

/* _____PREYS_____ */
int prey_next_position(
	int *current_board,
	int *next_board, 
	subject_t *subjects, 
	int id, int g, int x, int y, int R, int C
){

	// validate around positions in board limits
	int valids[4] = {0,0,0,0};
	int count_valids = 0;
	get_valid_positions_around(current_board, x, y, R, C, (int*)&valids, &count_valids);

	// empty fields around
	int empty[count_valids];
	int count_empty = 0;
	get_empty_positions_around(current_board, valids, count_valids, (int*)&empty, &count_empty);

	// go to clear field
	if (count_empty){
		int pick = (g + x + y) % count_empty;
		return empty[pick];
	}

	// stay
	return x*C+y;
};

void move_preys(
	int *current_board,
	int *next_board, 
	subject_t *subjects,
	int g, int R, int C, int *N,
	int GEN_PREY
){

	for (int x = 0; x < R; x++) {
		for (int y = 0; y < C; y++) {

			int x_y = x*C+y;
			int id = current_board[x_y];
			
			if (subjects[id].type == PREY && subjects[id].prey.gen_proc <= GEN_PREY){
				int next_pos = prey_next_position(
					current_board, 
					next_board, 
					subjects,
					id, g, x, y, R, C
				);

				// check for conflict
				int alive = id;
				if (next_board[next_pos] >= 0){
					alive = solve_conflict(subjects, id, next_board[next_pos]);
					(*N)--;
				}

				if (alive == id){
					// move prey
					if (next_pos != x_y){
						next_board[next_pos] = id;
						next_board[x_y] = EMPTY_FIELD;
					}

					// reduce proc_gen and try to procreate
					subjects[id].prey.gen_proc--;
					int new_id = -1;
					if (subjects[id].prey.gen_proc < 0) {
						subjects[id].prey.gen_proc = GEN_PREY;
						if (x_y < next_pos)
							new_id = new_prey(subjects, R, C, GEN_PREY+1);
						else
							new_id = new_prey(subjects, R, C, GEN_PREY);
					}					
					// if procreation: add new prey to board and reset proc_count
					if (new_id != -1){
						next_board[x_y] = new_id;
						(*N)++;
						subjects[id].prey.gen_proc = GEN_PREY;
					}
				}
			}
			else if (subjects[id].type == PREY)
				subjects[id].prey.gen_proc--;
		}
	}
}

/* _____PREDATORS_____ */
int predator_next_position(
	int *current_board,
	int *next_board, 
	subject_t *subjects, 
	int id, int g, int x, int y, int R, int C
){

	// validate around positions in board limits
	int valids[4] = {0,0,0,0};
	int count_valids = 0;
	get_valid_positions_around(current_board, x, y, R, C, (int*)&valids, &count_valids);
	
	// preys around
	int prey[count_valids];
	int count_preys = 0;
	get_prey_positions_around(current_board, subjects, valids, count_valids, (int*)&prey, &count_preys);

	// attack prey
	if (count_preys){
		int pick = (g + x + y) % count_preys;
		return prey[pick];
	}

	// empty fields around
	int empty[count_valids];
	int count_empty = 0;
	get_empty_positions_around(current_board, valids, count_valids, (int*)&empty, &count_empty);

	// go to clear field
	if (count_empty){
		int pick = (g + x + y) % count_empty;
		return empty[pick];
	}

	// stay
	return x*C+y;
};

void move_predators(
	int *current_board,
	int *next_board, 
	subject_t *subjects,
	int g, int R, int C, int *N,
	int GEN_PREDATOR, int GEN_FOOD
){

	for (int x = 0; x < R; x++) {
		for (int y = 0; y < C; y++) {

			int x_y = x*C+y;
			int id = current_board[x_y];

			if (subjects[id].type == PREDATOR && subjects[id].predator.gen_proc <= GEN_PREDATOR){
				int next_pos = predator_next_position(
					current_board, 
					next_board, 
					subjects,
					id, g, x, y, R, C
				);

				// check for conflict
				int alive = id;
				if (next_board[next_pos] >= 0){
					alive = solve_conflict(subjects, id, next_board[next_pos]);
					(*N)--;
				}

				// try eat prey
				int other_id = current_board[next_pos];
				if (subjects[other_id].type == PREY){
					subjects[id].predator.gen_food = GEN_FOOD;
					subjects[other_id].type = NONE;
				}
				else
					subjects[id].predator.gen_food--;

				if (alive == id && subjects[id].predator.gen_food){
					// move predator
					if (next_pos != x_y){
						next_board[next_pos] = id;
						next_board[x_y] = EMPTY_FIELD;
					}

					// reduce proc_gen and try to procreate
					subjects[id].predator.gen_proc--;
					int new_id = -1;
					if (subjects[id].predator.gen_proc < 0) {
						subjects[id].predator.gen_proc = GEN_PREDATOR;
						if (x_y < next_pos)
							new_id = new_predator(subjects, R, C, GEN_PREDATOR+1, GEN_FOOD+1);
						else 
							new_id = new_predator(subjects, R, C, GEN_PREDATOR, GEN_FOOD);
					}					
					// if procreation: add new predator to board and reset proc_count
					if (new_id != -1){
						next_board[x_y] = new_id;
						(*N)++;
						subjects[id].predator.gen_proc = GEN_PREDATOR;
					}
				}
				// starvation
				else {
					subjects[id].type = NONE;
					next_board[x_y] = EMPTY_FIELD;
					(*N)--;
				}
			}
			else if (subjects[id].type == PREDATOR){
				subjects[id].predator.gen_food--;
				subjects[id].predator.gen_proc--;
			}
		}
	}
}
