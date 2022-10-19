#include "compute.h"


void compute_array(int* array, int size, int rank){
	for (int i = 0; i < size; i++)
		array[i] = compute_value(array[i]);
}

int compute_value(int value){

	int count = -2;

	int increment = (value % 2 == 0) ? 1 : 2;

	for (int i = 1; i <= value; i+=increment)
		if (value % i == 0)
			count++;

	return count;
}
