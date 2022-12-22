#include "compute.h"
#include <stdio.h>


void compute_array(int* array, int size){
	#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < size; i++)
		array[i] = compute_value(array[i]);
}

int compute_value(int value){

    // count divisors. Starts w/ -1 because value%1 == 0
	int count = -1;

    // if its a odd number, increments 2 by 2 (1,3,5,...). Odd numbers %2 == 1
	int increment = (value % 2 == 0) ? 1 : 2;

	for (int i = 1; i <= (int)(value/2); i+=increment)
		if (value % i == 0)
			count++;

	return count;
}
