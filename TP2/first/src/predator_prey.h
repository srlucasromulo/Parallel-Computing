#ifndef __PREDATOR_PREY__
#define __PREDATOR_PREY__


#define PREDATOR 'X'
#define PREY 'O'
#define OBSTACLE '-'
#define EMPTY ' '

#define TRUE  1
#define FALSE 0

// definir subject e usar union>?

typedef struct predator_t {
	int gen_proc;
	int gen_food;
} predator_t;

typedef struct prey_t {
	int gen_proc;
} prey_t;

typedef board_position_t {
	char type = EMPTY;
	int id = 0;
} board_position_t;


#endif
