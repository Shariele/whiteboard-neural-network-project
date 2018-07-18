/**************************************************************************/
/*	INCLUDES	 														  */
/**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**************************************************************************/
/*	IMPORT METHODS	 													  */
/**************************************************************************/
#include "neural-network.h"

/**************************************************************************/
/*	TYPE DEFINITIONS & VARIABLES										  */
/**************************************************************************/
#define DEBUG 1

static matrices trainData[1];

/**************************************************************************/
/*	METHODS														  		  */
/**************************************************************************/

int main(int argc, char const *argv[]){
	int i, x, y, countTD = 0;
	trainData[0].number = 1;
	trainData[0].id = 0;

	if(DEBUG){

		// Print matrix
		printf("\nCountTD: %d\n", countTD);
		printf("\nId for matrix: %d \tNumber for matrix: %d\n", trainData[countTD].id, trainData[countTD].number);
		for(i = 0; i < MATRIXSIZE; i++)
			printf("%2d ", i);
		printf("\n");
		for(y = 0; y < MATRIXSIZE; y++){
			for(x = 0; x < MATRIXSIZE; x++){
				printf("%2d ", trainData[countTD].matrix[x][y]);
			}
			printf("\n");
		}
		printf("\n");
	}

	neuralNetwork(trainData);

	return 0;
}