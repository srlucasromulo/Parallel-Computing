#include "predator_prey.h"
#include <stdio.h>


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
	const subject_t *subjects, int N,
	int x, int y, int R, int C,
	int *empty, int *count_empty)
{

	for (int i = 0; i < 4; i++){
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, i);
		if (is_board_position(x_, y_, R, C))
			if (subject_in_position(subjects, N, x_, y_) == NONE)
				empty[(*count_empty)++] = i;
	}
}

void get_prey_positions_around(
	const subject_t *subjects, int N,
	int x, int y, int R, int C,
	int *prey, int *count_prey)
{

	for (int i = 0; i < 4; i++){
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, i);
		if (is_board_position(x_, y_, R, C)){
			char type = subject_in_position(subjects, N, x_, y_);
			if (type == PREY)
				prey[(*count_prey)++] = i;
		}
	}
}

/* _____PREYS_____ */
void prey_next_position(
	subject_t *subject, 
	const subject_t *subjects,
	int g, int R, int C, int N
){

	int x = subject->x;
	int y = subject->y;

	// empty fields around
	int empty[4] = {0,0,0,0};
	int count_empty = 0;
	get_empty_positions_around(subjects, N, x, y, R, C, (int*)&empty, &count_empty);

	// go to clear field
	if (count_empty){
		int pick = (g + x + y) % count_empty;
		int x_, y_;
		transform_by_orientation(x, y, &x_, &y_, empty[pick]);
		subject->x = x_;
		subject->y = y_;
	}
};

void move_preys(
	const subject_t *subjects, subject_t *next_subjects,
	int g, int R, int C, int *N,
	int GEN_PREY
){
	clock_t time = clock();

	int n_ = 0;
	const int fix_N = *N;

	for (int id = 0; n_ < fix_N; id++){

		if (subjects[id].type != NONE)
			n_++;

		if (subjects[id].type == PREY){

			subject_t *subject = &next_subjects[id];

			prey_next_position(subject, subjects, g, R, C, fix_N);

			if (subject->prey.gen_proc-- == 0){
				subject_t new = new_prey(subjects[id].x, subjects[id].y, GEN_PREY);
				next_subjects[(*N)++] = new;
				subject->prey.gen_proc = GEN_PREY;
			}
		}
	}

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}

/* _____PREDATORS_____ */
int attack_prey(
	subject_t *subject, 
	const subject_t *subjects,
	int g, int R, int C, int N
){
	int x = subject->x;
	int y = subject->y;

	// preys around
	int prey[4] = {0,0,0,0};
	int count_prey = 0;
	get_prey_positions_around(subjects, N, x, y, R, C, (int*)&prey, &count_prey);

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
	int g, int R, int C, int N
){

	int x = subject->x;
	int y = subject->y;

	// empty fields around
	int empty[4] = {0,0,0,0};
	int count_empty = 0;
	get_empty_positions_around(subjects, N, x, y, R, C, (int*)&empty, &count_empty);

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
	int GEN_PREDATOR, int GEN_FOOD
){
	clock_t time = clock();

	int n_ = 0;
	const int fix_N = *N;

	for (int id = 0; n_ < fix_N; id++){

		if (subjects[id].type != NONE)
			n_++;

		if (subjects[id].type == PREDATOR){

			subject_t *subject = &next_subjects[id];

			if (attack_prey(subject, subjects, g, R, C, fix_N)){
				subject->predator.gen_food = GEN_FOOD;
			}
			else {
				if (--subject->predator.gen_food == 0){
					subject->type = NONE;
					(*N)--;
				}
				else{
					predator_next_position(subject, subjects, g, R, C, fix_N);

					if (subject->predator.gen_proc-- == 0){
						subject_t new = new_predator(subjects[id].x, subjects[id].y, GEN_PREDATOR, GEN_FOOD);
						next_subjects[(*N)++] = new;
						subject->prey.gen_proc = GEN_PREDATOR;
					}
				}
			}
		}
	}

	time = clock() - time;
	printf("%s;%lf\n", __FUNCTION__, (double)time/CLOCKS_PER_SEC);
}

/* _____CONFLICT_____ */
void solve_conflicts(
	const subject_t *subjects,
	subject_t *next_subjects,
	int *N
){

	// TODO

}
