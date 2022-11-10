#include "predator_prey.h"
#include <stdio.h>


enum directions {NORTH=1, EAST, SOUTH, WEST};

// count-- subjects gens and kills predators
void decrease_gen(
	int *board, 
	subject_t *subjects, 
	int R, int C, int *N
) {

	for (int i = 0; i < R * C; i++){
		if (board[i] >= 0){

			// decrease all subjects gens
			int id = board[i];
			if (subjects[id].type == PREDATOR){
				subjects[id].predator.gen_food--;
				subjects[id].predator.gen_proc--;
			}
			if (subjects[id].type == PREY) 
				subjects[id].prey.gen_proc--;

			// kill predators
			if (subjects[id].type == PREDATOR)
				if (subjects[id].predator.gen_food == 0){
					board[i] = EMPTY_FIELD;
					subjects[id].type = NONE;
					(*N)--;
				}
		}
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

	// DBG
	printf("%c: (%d, %d)\n", subjects[id].type, x, y);

	// positions around
	int around[4][2] = {{x-1,y}, {x,y+1}, {x+1,y}, {x,y-1}};

	// validate around positions in board limits
	int valids[4] = {0,0,0,0};
	int count_valids = 0;
	for (int i = 0; i < 4; i++)
		if (is_board_position(around[i][0], around[i][1], R, C))
			valids[count_valids++] = around[i][0]*C+around[i][1];

	// DBG
	printf("valids(%d): ", count_valids);
	for (int i = 0; i < count_valids; i++)
		printf("(%d, %d), ", (int)valids[i]/C, valids[i]%R);
	printf("\n");

	// predators try to catch preys
	if (subjects[id].type == PREDATOR){

		int preys[count_valids];
		int count_preys = 0;

		// check valid around positions for preys 
		for (int i = 0; i < count_valids; i++)
			if (current_board[valids[i]] >= 0){
				int o = current_board[valids[i]];
				if (subjects[o].type == PREY)
					preys[count_preys++] = valids[i];
			}

		// DBG
		printf("preys(%d): ", count_preys);
		for (int i = 0; i < count_preys; i++)
			printf("(%d, %d), ", (int)preys[i]/C, preys[i]%R);
		printf("\n");

		// follow prey
		if (count_preys){
			int pick = (g + x + y) % count_preys;
		printf("(g+x+y)modP=(%d+%d+%d)mod%d = %d -> (%d, %d)\n", g,x,y,count_preys, pick, preys[pick]/C, preys[pick]%R);
			return preys[pick];
		}
	}

	// empty fields around
	int empty[count_valids];
	int count_empty = 0;
	for (int i = 0; i < count_valids; i++)
		if (current_board[valids[i]] == EMPTY_FIELD)
			empty[count_empty++] = valids[i];

	// DBG
	printf("empty(%d): ", count_empty);
	for (int i = 0; i < count_empty; i++)
		printf("(%d, %d), ", (int)empty[i]/C, empty[i]%R);
	printf("\n");

	// go to clear field
	if (count_empty){
		int pick = (g + x + y) % count_empty;
		printf("(g+x+y)modP=(%d+%d+%d)mod%d = %d -> (%d, %d)\n", g,x,y,count_empty, pick, empty[pick]/C, empty[pick]%R);
		return empty[pick];
	}

	// stay
	printf("stay: (%d, %d)\n", x, y);
	return x*C+y;
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

void move_subjects(
	int *current_board, 
	int *next_board, 
	subject_t *subjects,
	int g, int R, int C, int *N,
	int GEN_PREDATOR, int GEN_PREY, int GEN_FOOD 
) {

	decrease_gen(current_board, subjects, R, C, N);

	for (int x = 0; x < R; x++) {
		for (int y = 0; y < C; y++) {

			int x_y = x*C+y;

			if (current_board[x_y] >= 0){

				int id = current_board[x_y];

				int next_pos = get_next_position(
					current_board, 
					next_board, 
					subjects,
					id, g, x, y, R, C
				);
				printf("\n");

				// check for conflict
				int alive = id;
				if (next_board[next_pos] >= 0){
					alive = solve_conflict(subjects, id, next_board[next_pos]);
					(*N)--;
				}

				if (alive == id){

					// move subject
					if (next_pos != x_y){
						next_board[next_pos] = id;
						next_board[x_y] = EMPTY_FIELD;
					}

					// try to procreate
					int new_id = -1; // id of the new subject
					if (subjects[id].type == PREDATOR)
						if (subjects[id].predator.gen_proc < 0){
							subjects[id].predator.gen_proc = GEN_PREDATOR;
							new_id = new_predator(subjects, R, C, GEN_PREDATOR, GEN_FOOD);
						}
					if (subjects[id].type == PREY)
						if (subjects[id].prey.gen_proc < 0) {
							subjects[id].prey.gen_proc = GEN_PREY;
							new_id = new_prey(subjects, R, C, GEN_PREY);
						}
					// if procreation: add new subject to board
					if (new_id != -1){
						next_board[x_y] = new_id;
						(*N)++;
					}
				}
			}
		}
	}
}
