#include "predator_prey.h"
#include <omp.h>
#include <stdio.h>
#include <time.h>


enum directions {NORTH=0, EAST, SOUTH, WEST};


int is_board_position(int x, int y, int R, int C){
	if (x >= 0 && x < R)
		if (y >= 0 && y < C)
			return 1;
	return 0;
}

void transform_by_orientation(int x, int y, int *x_, int *y_, int orient){

	*x_ = x;
	*y_ = y;

	if (orient == NORTH) (*x_)--;
	if (orient == EAST)  (*y_)++;
	if (orient == SOUTH) (*x_)++;
	if (orient == WEST)  (*y_)--;
}

void get_empty_positions_around(
	const subject_t *subjects,
	int x, int y, int R, int C,
	int *empty, int *count_empty)
{
	// save empty fields around in a vector using orientation (N=0, E=1, S=2, W=3)
	for (int i = 0; i < 4; i++){
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, i);
		if (is_board_position(x_, y_, R, C))
			if (subject_in_position(subjects, R*C, x_, y_) == NONE)
				empty[(*count_empty)++] = i;
	}
}

void get_prey_positions_around(
	const subject_t *subjects,
	int x, int y, int R, int C,
	int *prey, int *count_prey)
{
	// save preys around in a vector using orientation
	for (int i = 0; i < 4; i++){
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, i);
		if (is_board_position(x_, y_, R, C)){
			char type = subject_in_position(subjects, R*C, x_, y_);
			if (type == PREY)
				prey[(*count_prey)++] = i;
		}
	}
}


/* _____PREYS_____ */
void prey_next_position(
	subject_t *subject, 
	const subject_t *subjects,
	int g, int R, int C
){

	int x = subject->x;
	int y = subject->y;

	// empty fields around
	int empty[4] = {0,0,0,0};
	int count_empty = 0;
	get_empty_positions_around(subjects, x, y, R, C, (int*)&empty, &count_empty);

	// go to empty field
	if (count_empty){
		int pick = (g + x + y) % count_empty;
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, empty[pick]);
		subject->x = x_;
		subject->y = y_;
	}
};

void move_preys(
	subject_t *subjects, subject_t *next_subjects,
	int g, int R, int C, int *N,
	int GEN_PROC
){
	clock_t time = clock();

	int N_tmp = *N;

	for (int id = 0; id < R*C; id++){

		// for each prey
		if (subjects[id].type == PREY){

			subject_t *subject = &next_subjects[id];

			// move
			prey_next_position(subject, subjects, g, R, C);

			// try procreation
			if (subject->prey.gen_proc-- == 0){
				subject_t new = new_prey(subjects[id].x, subjects[id].y, GEN_PROC);
				add_subject_to_list(next_subjects, new);
				N_tmp++;
				subject->prey.gen_proc = GEN_PROC;
			}
		}
	}
	*N = N_tmp;
	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}


/* _____PREDATORS_____ */
int attack_prey(
	subject_t *subject, 
	const subject_t *subjects,
	int g, int R, int C
){
	int x = subject->x;
	int y = subject->y;

	// preys around
	int prey[4] = {0,0,0,0};
	int count_prey = 0;
	get_prey_positions_around(subjects, x, y, R, C, (int*)&prey, &count_prey);

	// attack
	if (count_prey){
		int pick = (g + x + y) % count_prey;
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, prey[pick]);
		subject->x = x_;
		subject->y = y_;
		return 1;
	}

	return 0;
}

void predator_next_position(
	subject_t *subject, 
	const subject_t *subjects,
	int g, int R, int C
){

	int x = subject->x;
	int y = subject->y;

	// empty fields around
	int empty[4] = {0,0,0,0};
	int count_empty = 0;
	get_empty_positions_around(subjects, x, y, R, C, (int*)&empty, &count_empty);

	// go to clear field
	if (count_empty){
		int pick = (g + x + y) % count_empty;
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, empty[pick]);
		subject->x = x_;
		subject->y = y_;
	}
};

void move_predators(
	const subject_t *subjects, subject_t *next_subjects,
	int g, int R, int C, int *N,
	int GEN_PROC, int GEN_FOOD
){
	clock_t time = clock();

	int N_tmp = *N;

	for (int id = 0; id < R*C; id++){

		// for each predator
		if (subjects[id].type == PREDATOR){

			subject_t *subject = &next_subjects[id];

			int alive = 1;
			// try attack
			if (attack_prey(subject, next_subjects, g, R, C)){
				subject->predator.gen_food = GEN_FOOD;
			}
			else {
				// check for starvation
				if (--subject->predator.gen_food == 0){
					subject->type = NONE;
					N_tmp--;
					alive = 0;
				}
				// move
				else{
					predator_next_position(subject, subjects, g, R, C);
				}
			}
			// try procreation
			if (alive && subject->predator.gen_proc-- == 0) {
				subject_t new = new_predator(subjects[id].x, subjects[id].y, GEN_PROC, GEN_FOOD);
				add_subject_to_list(next_subjects, new);
				N_tmp++;
				subject->predator.gen_proc = GEN_PROC;
			}
		}
	}
	*N = N_tmp;
	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}


/* _____CONFLICT_____ */
int solve(subject_t *s1, subject_t *s2){

	// case predator x prey
	if (s1->type != s2->type){
		if (s1->type == PREDATOR)
			return 1;
		return 2;
	}

	// case prey x prey
	if (s1->type == PREY && s2->type == PREY){
		if (s1->prey.gen_proc <= s2->prey.gen_proc)
			return 1;
		return 2;
	}

	// case predator x predator
	if (s1->type == PREDATOR && s2->type == PREDATOR){
		if (s1->predator.gen_proc != s2->predator.gen_proc){
			if (s1->predator.gen_proc < s2->predator.gen_proc)
				return 1;
			if (s2->predator.gen_proc < s1->predator.gen_proc)
				return 2;
		}
		if (s1->predator.gen_food <= s2->predator.gen_food)
			return 1;
		return 2;
	}

	// just to supress warnings
	return 0;
}

void solve_conflicts(
	subject_t *subjects,
	int size, int *N
){
	time_t time = clock();

	int N_tmp = *N;

	for (int i = 0; i < size; i++){
		for (int j = i+1; j < size; j++){

			// get subject1
			subject_t *sub1 = NULL;
			if (subjects[i].type == PREDATOR || subjects[i].type == PREY)
				sub1 = &subjects[i];

			// get subject2
			subject_t *sub2 = NULL;
			if (subjects[j].type == PREDATOR || subjects[j].type == PREY)
				sub2 = &subjects[j];

			int survivor = 0;
			// check sub1 & sub2 exist and are in the same position
			if (sub1 && sub2 && sub1->x == sub2->x && sub1->y == sub2->y) {
				if (sub1 && sub2 && sub1->x == sub2->x && sub1->y == sub2->y) 
					// solve and get the survivor
					survivor = solve(sub1, sub2);

				// erase the non-survivor from the list and decrease N
				if (survivor){
					if (survivor == 1){
						sub2->type = NONE;
						sub2 = NULL;
					}
					else{
						sub1->type = NONE;
						sub1 = NULL;
					}
					N_tmp--;
				}
			}
		}
	}
	*N = N_tmp;
	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}
